// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
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

#include "env.h"
#include "error.h"
#include "inet.h"

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
  struct ifreq ifr {
    .ifr_ifindex = ifindex,
  };
  int err = ::ioctl(IoctlFD().FD(), SIOCGIFNAME, &ifr);
  if (err < 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok(std::string(ifr.ifr_name));
}

inline Result<void> SetAddr(const char *ifname, const char *host) {
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

inline Result<void> SetDstAddr(const char *ifname, const char *host) {
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

inline Result<void> SetNetMask(const char *ifname, const char *mask) {
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

inline Result<void> SetMTU(const char *ifname, int mtu) {
  struct ifreq ifr {
    .ifr_mtu = mtu,
  };
  ::strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
  int err = ::ioctl(IoctlFD().FD(), SIOCSIFMTU, &ifr);
  if (err < 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok();
}

inline Result<void> AddRoute(const char *ifname, const char *host,
                             const char *mask) {
  auto host_addr = inet::InetSockAddr(host, 0);
  if (!host_addr) {
    return kl::Err(host_addr.MoveErr());
  }
  auto mask_addr = inet::InetSockAddr(mask, 0);
  if (!mask_addr) {
    return kl::Err(mask_addr.MoveErr());
  }
  struct rtentry rt;
  ::memset(&rt, 0, sizeof(rt));
  rt.rt_dst = *reinterpret_cast<struct sockaddr *>(&(*host_addr));
  rt.rt_genmask = *reinterpret_cast<struct sockaddr *>(&(*mask_addr));
  char dev[IFNAMSIZ];
  ::strncpy(dev, ifname, IFNAMSIZ - 1);
  rt.rt_dev = dev;
  rt.rt_flags = RTF_UP | RTF_HOST;
  int err = ::ioctl(IoctlFD().FD(), SIOCADDRT, &rt);
  if (err < 0) {
    return kl::Err(errno, std::strerror(errno));
  }
  return kl::Ok();
}

inline Result<void> AddGateway(const char *ifname, const char *host,
                               const char *mask) {
  auto host_addr = inet::InetSockAddr(host, 0);
  if (!host_addr) {
    return kl::Err(host_addr.MoveErr());
  }
  auto mask_addr = inet::InetSockAddr(mask, 0);
  if (!mask_addr) {
    return kl::Err(mask_addr.MoveErr());
  }
  struct rtentry rt;
  ::memset(&rt, 0, sizeof(rt));
  rt.rt_dst = *reinterpret_cast<struct sockaddr *>(&(*host_addr));
  rt.rt_genmask = *reinterpret_cast<struct sockaddr *>(&(*mask_addr));
  char dev[IFNAMSIZ];
  ::strncpy(dev, ifname, IFNAMSIZ - 1);
  rt.rt_dev = dev;
  rt.rt_flags = RTF_GATEWAY | RTF_HOST;
  int err = ::ioctl(IoctlFD().FD(), SIOCADDRT, &rt);
  if (err < 0) {
    return kl::Err(errno, std::strerror(errno));
  }
  return kl::Ok();
}

inline Result<void> AddNetRoute(const char *ifname, const char *host,
                                const char *mask) {
  auto host_addr = inet::InetSockAddr(host, 0);
  if (!host_addr) {
    return kl::Err(host_addr.MoveErr());
  }
  auto mask_addr = inet::InetSockAddr(mask, 0);
  if (!mask_addr) {
    return kl::Err(mask_addr.MoveErr());
  }
  struct rtentry rt;
  ::memset(&rt, 0, sizeof(rt));
  rt.rt_dst = *reinterpret_cast<struct sockaddr *>(&(*host_addr));
  rt.rt_genmask = *reinterpret_cast<struct sockaddr *>(&(*mask_addr));
  char dev[IFNAMSIZ];
  ::strncpy(dev, ifname, IFNAMSIZ - 1);
  rt.rt_dev = dev;
  rt.rt_flags = RTF_UP;
  int err = ::ioctl(IoctlFD().FD(), SIOCADDRT, &rt);
  if (err < 0) {
    return kl::Err(errno, std::strerror(errno));
  }
  return kl::Ok();
}

inline Result<void> InterfaceUp(const char *ifname) {
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

inline Result<void> BindInterface(int fd, const char *ifname) {
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
