// Copyright (c) 2016 Kai Luo. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "epoll.h"

namespace kl {

Epoll::Epoll() {
  // Since Linux 2.6.8, the size argument is ignored, but must be
  // greater than zero
  epfd_ = epoll_create(1);
  if (epfd_ < 0) {
    throw std::runtime_error(static_cast<const char *>(std::strerror(errno)));
  }
}

Status Epoll::AddFd(int fd, uint32_t flags) {
  struct epoll_event event = {.events = flags, .data = {.fd = fd}};
  int ret = epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &event);
  if (ret != 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok();
}

Status Epoll::ModFd(int fd, uint32_t flags) {
  struct epoll_event event = {.events = flags, .data = {.fd = fd}};
  int ret = epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &event);
  if (ret != 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok();
}

Status Epoll::DelFd(int fd) {
  int ret = epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr);
  if (ret != 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok();
}

Result<std::vector<struct epoll_event>> Epoll::Wait(int max_events,
                                                    int timeout) {
  std::vector<struct epoll_event> events(max_events);
  int ret = epoll_wait(epfd_, events.data(), events.size(), timeout);
  if (ret < 0) {
    return Err(errno, std::strerror(errno));
  }
  events.resize(ret);
  return Ok(std::move(events));
}

}  // namespace kl
