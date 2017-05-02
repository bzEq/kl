// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "kl/epoll.h"
#include "kl/logger.h"
#include "kl/testkit.h"
#include "kl/udp.h"

namespace {
class T {};

TEST(T, EpollTimeout) {
  kl::Epoll ep;
  int fd = *kl::udp::Socket();
  ASSERT(ep.AddFd(fd, EPOLLET | EPOLLIN));
  auto wait = ep.Wait(10, 1000);
  if (!wait) {
    KL_DEBUG(wait.Err().ToCString());
  }
  ASSERT(wait);
  KL_DEBUG("number of events %d", (*wait).size());
}
}  // namespace
