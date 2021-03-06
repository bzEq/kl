// Copyright (c) 2016 Kai Luo. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_EPOLL_H_
#define KL_EPOLL_H_
#include <sys/epoll.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <thread>
#include <vector>

#include "kl/error.h"

namespace kl {

class Epoll {
public:
  Epoll();
  Epoll(const Epoll &) = delete;
  Epoll(Epoll &&) = default;
  Status AddFd(int fd, uint32_t flags);
  Status ModFd(int fd, uint32_t flags);
  Status DelFd(int fd);
  Result<std::vector<struct epoll_event>> Wait(int max_events, int timeout);
  ~Epoll() {
    if (epfd_ >= 0) {
      close(epfd_);
    }
  }

private:
  int epfd_;
};
}  // namespace kl

#endif
