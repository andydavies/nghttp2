/*
 * nghttp2 - HTTP/2 C Library
 *
 * Copyright (c) 2012 Tatsuhiro Tsujikawa
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef SHRPX_CLIENT_HANDLER_H
#define SHRPX_CLIENT_HANDLER_H

#include "shrpx.h"

#include <memory>

#include <event.h>
#include <event2/bufferevent.h>

#include <openssl/ssl.h>

namespace shrpx {

class Upstream;
class DownstreamConnection;
class Http2Session;
class HttpsUpstream;
class ConnectBlocker;
class DownstreamConnectionPool;
struct WorkerStat;

class ClientHandler {
public:
  ClientHandler(bufferevent *bev,
                bufferevent_rate_limit_group *rate_limit_group,
                int fd, SSL *ssl, const char *ipaddr, const char *port,
                WorkerStat *worker_stat,
                DownstreamConnectionPool *dconn_pool);
  ~ClientHandler();
  int on_read();
  int on_event();
  bufferevent* get_bev() const;
  event_base* get_evbase() const;
  void set_bev_cb(bufferevent_data_cb readcb, bufferevent_data_cb writecb,
                  bufferevent_event_cb eventcb);
  void set_upstream_timeouts(const timeval *read_timeout,
                             const timeval *write_timeout);
  int validate_next_proto();
  const std::string& get_ipaddr() const;
  const std::string& get_port() const;
  bool get_should_close_after_write() const;
  void set_should_close_after_write(bool f);
  Upstream* get_upstream();

  void pool_downstream_connection(std::unique_ptr<DownstreamConnection> dconn);
  void remove_downstream_connection(DownstreamConnection *dconn);
  std::unique_ptr<DownstreamConnection> get_downstream_connection();
  size_t get_outbuf_length();
  SSL* get_ssl() const;
  void set_http2_session(Http2Session *http2session);
  Http2Session* get_http2_session() const;
  void set_http1_connect_blocker(ConnectBlocker *http1_connect_blocker);
  ConnectBlocker* get_http1_connect_blocker() const;
  // Call this function when HTTP/2 connection header is received at
  // the start of the connection.
  void direct_http2_upgrade();
  // Performs HTTP/2 Upgrade from the connection managed by
  // |http|. If this function fails, the connection must be
  // terminated. This function returns 0 if it succeeds, or -1.
  int perform_http2_upgrade(HttpsUpstream *http);
  bool get_http2_upgrade_allowed() const;
  // Returns upstream scheme, either "http" or "https"
  std::string get_upstream_scheme() const;
  void set_tls_handshake(bool f);
  bool get_tls_handshake() const;
  void set_tls_renegotiation(bool f);
  bool get_tls_renegotiation() const;
  int on_http2_connhd_read();
  int on_http1_connhd_read();
  // Returns maximum chunk size for one evbuffer_add().  The intention
  // of this chunk size is control the TLS record size.  The actual
  // SSL_write() call is done under libevent control.  In
  // libevent-2.0.21, libevent calls SSL_write() for each chunk inside
  // evbuffer.  This means that we can control TLS record size by
  // adjusting the chunk size to evbuffer_add().
  //
  // This function returns -1, if TLS is not enabled or no limitation
  // is required.
  ssize_t get_write_limit();
  // Updates the number of bytes written in warm up period.
  void update_warmup_writelen(size_t n);
  // Updates the time when last write was done.
  void update_last_write_time();

  // Writes upstream accesslog using |downstream|.  The |downstream|
  // must not be nullptr.
  void write_accesslog(Downstream *downstream);

  // Writes upstream accesslog.  This function is used if
  // corresponding Downstream object is not available.
  void write_accesslog(int major, int minor, unsigned int status,
                       int64_t body_bytes_sent);
private:
  std::unique_ptr<Upstream> upstream_;
  std::string ipaddr_;
  std::string port_;
  // The ALPN identifier negotiated for this connection.
  std::string alpn_;
  DownstreamConnectionPool *dconn_pool_;
  bufferevent *bev_;
  // Shared HTTP2 session for each thread. NULL if backend is not
  // HTTP2. Not deleted by this object.
  Http2Session *http2session_;
  ConnectBlocker *http1_connect_blocker_;
  SSL *ssl_;
  event *reneg_shutdown_timerev_;
  WorkerStat *worker_stat_;
  int64_t last_write_time_;
  size_t warmup_writelen_;
  // The number of bytes of HTTP/2 client connection header to read
  size_t left_connhd_len_;
  int fd_;
  bool should_close_after_write_;
  bool tls_handshake_;
  bool tls_renegotiation_;
};

} // namespace shrpx

#endif // SHRPX_CLIENT_HANDLER_H
