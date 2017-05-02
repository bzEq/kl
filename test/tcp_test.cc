// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "kl/tcp.h"
#include "kl/env.h"
#include "kl/inet.h"
#include "kl/testkit.h"

namespace {
class T {};

TEST(T, ListenAndSetNonBlocking) {
  auto listen = kl::tcp::Listen("127.0.0.1", 4000);
  ASSERT(listen);
  kl::env::Defer defer([fd = *listen]() { ::close(fd); });
  ASSERT(kl::env::SetNonBlocking(*listen));
}

TEST(T, Rebind) {
  auto sock0 = kl::tcp::Socket();
  ASSERT(sock0);
  kl::env::Defer defer([fd = *sock0]() { ::close(fd); });
  auto bind0 = kl::inet::Bind(*sock0, "127.0.0.1", 4000);
  ASSERT(bind0);
  auto sock1 = kl::tcp::Socket();
  ASSERT(sock1);
  defer([fd = *sock1]() { ::close(fd); });
  auto bind1 = kl::inet::Bind(*sock1, "127.0.0.1", 4000);
  ASSERT(!bind1);
}

TEST(T, Connect) {
  auto listen = kl::tcp::Listen("127.0.0.1", 4000);
  ASSERT(listen);
  kl::env::Defer defer([fd = *listen]() { ::close(fd); });
  auto connect = kl::tcp::BlockingConnect("127.0.0.1", 4000);
  ASSERT(connect);
  defer([fd = *connect]() { ::close(fd); });
}
}  // namespace
