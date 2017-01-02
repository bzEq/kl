// Copyright (c) 2016 Kai Luo. All rights reserved.

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

#include "error.h"

namespace kl {

class Epoll {
public:
  Epoll();
  Epoll(const Epoll &) = delete;
  Epoll(Epoll &&) = default;
  Result<void> AddFd(int fd, uint32_t flags);
  Result<void> ModFd(int fd, uint32_t flags);
  Result<void> DelFd(int fd);
  Result<std::vector<struct epoll_event>> Wait(int, int);
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
