// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

// Manipulate NIC, so that programs can be free from ifconfig
#ifndef KL_NETDEV_H_
#define KL_NETDEV_H_
#include <arpa/inet.h>
#include <fcntl.h>
#include <net/if.h>
#include <net/route.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "kl/env.h"
#include "kl/error.h"
#include "kl/inet.h"

namespace kl {
namespace netdev {

namespace {
// Used for one shot ioctl call.
class IoctlFD {
public:
  IoctlFD() : fd_(-1) {
    fd_ = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (fd_ < 0) {
      throw std::runtime_error(std::strerror(errno));
    }
  }
  int FD() const { return fd_; }
  void Close() {
    if (fd_ >= 0) {
      ::close(fd_);
      fd_ = -1;
    }
  }
  ~IoctlFD() { Close(); }

private:
  int fd_;
};
}

inline Result<std::vector<struct ifreq>> ListIPv4Interfaces() {
  int fd = ::socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    return Err(errno, std::strerror(errno));
  }
  env::Defer defer([fd]() { ::close(fd); });
  struct ifconf conf;
  conf.ifc_len = 0;
  conf.ifc_req = nullptr;
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

inline Status PrintIPv4Interfaces(std::ostream &out) {
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

inline Result<int> RetrieveIFIndex(const char *ifname) {
  struct ifreq ifr;
  ::strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
  int err = ::ioctl(IoctlFD().FD(), SIOCGIFINDEX, &ifr);
  if (err < 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok(ifr.ifr_ifindex);
}

inline Result<std::string> RetrieveIFName(int ifindex) {
  struct ifreq ifr;
  ifr.ifr_ifindex = ifindex;
  int err = ::ioctl(IoctlFD().FD(), SIOCGIFNAME, &ifr);
  if (err < 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok(std::string(ifr.ifr_name));
}

inline Status SetAddr(const char *ifname, const char *host) {
  struct ifreq ifr;
  ::strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
  struct sockaddr_in &addr =
      *reinterpret_cast<struct sockaddr_in *>(&ifr.ifr_addr);
  addr = {
      .sin_family = AF_INET, .sin_port = 0,
  };
  int err = ::inet_aton(host, &addr.sin_addr);
  if (!err) {
    return kl::Err("invalid ip address: %s", host);
  }
  err = ::ioctl(IoctlFD().FD(), SIOCSIFADDR, &ifr);
  if (err < 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok();
}

inline Status SetDstAddr(const char *ifname, const char *host) {
  struct ifreq ifr;
  ::strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
  struct sockaddr_in &addr =
      *reinterpret_cast<struct sockaddr_in *>(&ifr.ifr_dstaddr);
  addr = {
      .sin_family = AF_INET, .sin_port = 0,
  };
  int err = ::inet_aton(host, &addr.sin_addr);
  if (!err) {
    return kl::Err("invalid ip address: %s", host);
  }
  err = ::ioctl(IoctlFD().FD(), SIOCSIFDSTADDR, &ifr);
  if (err < 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok();
}

inline Result<std::string> GetAddr(const char *ifname) {
  struct ifreq ifr;
  ::strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
  int err = ::ioctl(IoctlFD().FD(), SIOCGIFADDR, &ifr);
  if (err < 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok(std::string(inet_ntoa(
      reinterpret_cast<struct sockaddr_in *>(&ifr.ifr_addr)->sin_addr)));
}

inline Status SetNetMask(const char *ifname, const char *mask) {
  struct ifreq ifr;
  ::strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
  struct sockaddr_in &addr =
      *reinterpret_cast<struct sockaddr_in *>(&ifr.ifr_addr);
  addr = {
      .sin_family = AF_INET, .sin_port = 0,
  };
  int err = ::inet_aton(mask, &addr.sin_addr);
  if (!err) {
    return kl::Err("invalid mask address: %s", mask);
  }
  err = ::ioctl(IoctlFD().FD(), SIOCSIFNETMASK, &ifr);
  if (err < 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok();
}

inline Result<int> GetMTU(const char *ifname) {
  struct ifreq ifr;
  ::strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
  int err = ::ioctl(IoctlFD().FD(), SIOCGIFMTU, &ifr);
  if (err < 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok(ifr.ifr_mtu);
}

inline Status SetMTU(const char *ifname, int mtu) {
  struct ifreq ifr;
  ifr.ifr_mtu = mtu;
  ::strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
  int err = ::ioctl(IoctlFD().FD(), SIOCSIFMTU, &ifr);
  if (err < 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok();
}

// route add default gw <addr>
// REQUIRES: addr must be address of AF_INET
inline Status AddDefaultGateway(const char *addr) {
  struct rtentry rt = {};
  auto inet_addr = inet::InetSockAddr(addr, 0);
  if (!inet_addr) {
    return kl::Err(inet_addr.MoveErr());
  }
  auto inet_dst = *inet::InetSockAddr("0.0.0.0", 0);
  rt.rt_dst = *reinterpret_cast<struct sockaddr *>(&inet_dst);
  rt.rt_gateway = *reinterpret_cast<struct sockaddr *>(&(*inet_addr));
  rt.rt_flags = RTF_UP | RTF_GATEWAY;
  int err = ::ioctl(IoctlFD().FD(), SIOCADDRT, &rt);
  if (err < 0) {
    return kl::Err(errno, std::strerror(errno));
  }
  return kl::Ok();
}

// route add -host <dst> gw <gateway> dev <ifname>
// REQUIRES: dst != nullptr
inline Status AddRoute(const char *dst, const char *gateway,
                       const char *ifname) {
  assert(dst);
  struct rtentry rt = {};
  auto inet_dst = inet::InetSockAddr(dst, 0);
  if (!inet_dst) {
    return kl::Err(inet_dst.MoveErr());
  }
  rt.rt_dst = *reinterpret_cast<struct sockaddr *>(&(*inet_dst));
  rt.rt_flags |= RTF_UP | RTF_HOST;
  if (gateway) {
    auto inet_gateway = inet::InetSockAddr(gateway, 0);
    if (inet_gateway) {
      rt.rt_gateway = *reinterpret_cast<struct sockaddr *>(&(*inet_gateway));
      rt.rt_flags |= RTF_GATEWAY;
    }
  }
  if (ifname) {
    char dev[IFNAMSIZ];
    ::strncpy(dev, ifname, IFNAMSIZ - 1);
    rt.rt_dev = dev;
  }
  int err = ::ioctl(IoctlFD().FD(), SIOCADDRT, &rt);
  if (err < 0) {
    return kl::Err(errno, std::strerror(errno));
  }
  return kl::Ok();
}

inline Status InterfaceUp(const char *ifname) {
  IoctlFD ioctl_fd;
  struct ifreq ifr;
  ::strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
  int err = ::ioctl(ioctl_fd.FD(), SIOCGIFFLAGS, &ifr);
  if (err < 0) {
    return kl::Err(errno, std::strerror(errno));
  }
  ifr.ifr_flags |= IFF_UP;
  err = ::ioctl(ioctl_fd.FD(), SIOCSIFFLAGS, &ifr);
  if (err < 0) {
    return kl::Err(errno, std::strerror(errno));
  }
  return kl::Ok();
}

inline Status BindInterface(int fd, const char *ifname) {
  int err =
      ::setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, ifname, ::strlen(ifname));
  if (err < 0) {
    return kl::Err(errno, std::strerror(errno));
  }
  return kl::Ok();
}

}  // namespace netdev
}  // namespace kl
#endif
