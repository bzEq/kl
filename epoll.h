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
  Epoll() {
    // Since Linux 2.6.8, the size argument is ignored, but must be
    // greater than zero
    epfd_ = epoll_create(1);
    if (epfd_ < 0) {
      throw std::runtime_error(static_cast<const char *>(std::strerror(errno)));
    }
  }
  Epoll(const Epoll &) = delete;
  Epoll(Epoll &&) = delete;
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
