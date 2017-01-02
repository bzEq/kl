// Copyright (c) 2016 Kai Luo. All rights reserved.

#include "epoll.h"

namespace kl {

Result<void> Epoll::AddFd(int fd, uint32_t flags) {
  struct epoll_event event = {.events = flags, .data = {.fd = fd}};
  int ret = epoll_ctl(epfd_, EPOLL_CTL_ADD, fd, &event);
  if (ret != 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok();
}

Result<void> Epoll::ModFd(int fd, uint32_t flags) {
  struct epoll_event event = {.events = flags, .data = {.fd = fd}};
  int ret = epoll_ctl(epfd_, EPOLL_CTL_MOD, fd, &event);
  if (ret != 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok();
}

Result<void> Epoll::DelFd(int fd) {
  int ret = epoll_ctl(epfd_, EPOLL_CTL_DEL, fd, nullptr);
  if (ret != 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok();
}

Result<std::vector<struct epoll_event>> Epoll::Wait(int maxEvents,
                                                    int timeout) {
  std::vector<struct epoll_event> events(maxEvents);
  int ret = epoll_wait(epfd_, events.data(), events.size(), timeout);
  if (ret < 0) {
    return Err(errno, std::strerror(errno));
  }
  events.resize(ret);
  return Ok(std::move(events));
}

}  // namespace kl
