// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Manipulate NIC, so that programs can be free from ifconfig
#ifndef KL_NETDEV_H_
#define KL_NETDEV_H_
#include <arpa/inet.h>
#include <fcntl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <vector>

#include "env.h"
#include "error.h"

namespace kl {
namespace netdev {

inline Result<std::vector<struct ifreq>> ListIPv4Interfaces() {
  int fd = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
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

inline Result<void> PrintIPv4Interfaces(std::ostream &out) {
  auto list = ListIPv4Interfaces();
  if (!list) {
    return Err(list.MoveErr());
  }
  for (const auto &ifreq : *list) {
    const char *ifname = ifreq.ifr_name;
    const char *addr =
        inet_ntoa(reinterpret_cast<const struct sockaddr_in *>(&ifreq.ifr_addr)
                      ->sin_addr);
    out << ifname << ": " << addr << "\n";
  }
  return Ok();
}

}  // namespace netdev
}  // namespace kl
#endif
