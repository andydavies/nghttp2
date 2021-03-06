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
#ifndef SHRPX_H
#define SHRPX_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif // HAVE_CONFIG_H

#include <sys/types.h>
#include <sys/socket.h>

#include <cassert>

#include "shrpx_log.h"

#ifndef SOCK_NONBLOCK
#define SOCK_NONBLOCK 0
#endif // !SOCK_NONBLOCK

#ifndef SOCK_CLOEXEC
#define SOCK_CLOEXEC 0
#endif // !SOCK_CLOEXEC

#ifndef HAVE__EXIT
#define _Exit(status) _exit(status)
#endif // !HAVE__EXIT

#define DIE() exit(EXIT_FAILURE)

#define SHRPX_READ_WATERMARK (16 * 1024)

#endif // SHRPX_H
