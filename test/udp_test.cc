// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include <string>

#include "kl/env.h"
#include "kl/inet.h"
#include "kl/testkit.h"
#include "kl/udp.h"

namespace {
class T {};

TEST(T, Socket) { ASSERT(kl::udp::Socket()); }

TEST(T, SendAndRecv) {
  const char *host = "127.0.0.1";
  uint16_t port = 4000;
  const std::string message("wtf");
  auto recv_sock = kl::udp::Socket();
  ASSERT(recv_sock);
  kl::env::Defer defer([fd = *recv_sock] { ::close(fd); });
  auto bind = kl::inet::Bind(*recv_sock, host, port);
  ASSERT(bind);
  auto send_sock = kl::udp::Socket();
  ASSERT(send_sock);
  defer([fd = *send_sock] { ::close(fd); });
  ASSERT(kl::inet::Sendto(*send_sock, message.c_str(), message.size(), 0, host,
                          port));
  char buf[65536];
  auto recv = kl::inet::RecvFrom(*recv_sock, buf, sizeof(buf), 0);
  ASSERT(recv);
  int nread = std::get<0>(*recv);
  buf[nread] = '\0';
  ASSERT(std::string(buf) == message);
}
}  // namespace
