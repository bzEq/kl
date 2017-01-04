// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Manipulate NIC, so that programs can be free from ifconfig
#ifndef KL_NETDEV_H_
#define KL_NETDEV_H_
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <vector>

#include "env.h"
#include "error.h"

namespace kl {
namespace netdev {

inline Result<std::vector<struct ifreq>> ListInterface() {
  int fd = ::socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    return Err(errno, std::strerror(errno));
  }
  env::Defer defer([fd]() { ::close(fd); });
  struct ifconf conf {
    .ifc_len = 0, .ifc_req = nullptr,
  };
  int err = ::ioctl(fd, SIOCGIFCONF, &conf);
  if (err < 0) {
    return Err(errno, std::strerror(errno));
  }
  assert(conf.ifc_len % sizeof(struct ifreq) == 0);
  size_t size = conf.ifc_len / sizeof(struct ifreq);
  std::vector<struct ifreq> ret(size);
  conf.ifc_req = &ret[0];
  err = ::ioctl(fd, SIOCGIFCONF, &conf);
  if (err < 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok(std::move(ret));
}

}  // namespace netdev
}  // namespace kl
#endif
