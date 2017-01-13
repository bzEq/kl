// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#include "epoll.h"
#include "logger.h"
#include "testkit.h"
#include "udp.h"

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

int main() { return KL_TEST(); }
