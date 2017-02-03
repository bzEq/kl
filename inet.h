// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_INET_H_
#define KL_INET_H_
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>

#include "error.h"
#include "string.h"

namespace kl {
namespace inet {

inline Status SplitAddr(const char *addr, std::string *host, uint16_t *port) {
  auto result = string::SplitString(addr, ":");
  if (result.size() != 2) {
    return Err("invalid addr: %s", addr);
  }
  *host = std::move(result[0]);
  *port = ::atoi(result[1].c_str());
  return Ok();
}

inline Result<struct sockaddr_in> InetSockAddr(const char *host,
                                               uint16_t port) {
  struct sockaddr_in addr = {
      .sin_family = AF_INET, .sin_port = htons(port),
  };
  int err = ::inet_aton(host, &addr.sin_addr);
  if (!err) {
    return kl::Err("invalid ip address: %s", host);
  }
  return kl::Ok(std::move(addr));
}

// RETURNS: (host, port)
inline std::tuple<std::string, uint16_t>
InetAddr(const struct sockaddr_in &addr) {
  std::string host(inet_ntoa(addr.sin_addr));
  uint16_t port(ntohs(addr.sin_port));
  return std::make_tuple(std::move(host), port);
}

inline Result<std::tuple<std::string, uint16_t>> InetAddr(int fd) {
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);
  int err = ::getsockname(fd, reinterpret_cast<struct sockaddr *>(&addr), &len);
  if (err < 0) {
    return kl::Err(errno, std::strerror(errno));
  }
  return kl::Ok(std::make_tuple(std::string(inet_ntoa(addr.sin_addr)),
                                ntohs(addr.sin_port)));
}

inline Status Bind(int fd, const char *host, uint16_t port) {
  struct sockaddr_in addr = {
      .sin_family = AF_INET, .sin_port = htons(port),
  };
  int err = ::inet_aton(host, &addr.sin_addr);
  if (!err) {
    return kl::Err("invalid ip address: %s", host);
  }
  err = ::bind(fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr));
  if (err != 0) {
    return kl::Err(errno, "can't bind the socket on port %u, %s", port,
                   std::strerror(errno));
  }
  return kl::Ok();
}

inline Status BlockingConnect(int fd, const char *host, uint16_t port) {
  auto addr = InetSockAddr(host, port);
  int err = ::connect(fd, reinterpret_cast<const struct sockaddr *>(&(*addr)),
                      sizeof(*addr));
  if (err < 0) {
    ::close(fd);
    return kl::Err(errno, std::strerror(errno));
  }
  return kl::Ok();
}

inline Status Listen(int fd, const char *host, uint16_t port) {
  auto bind = Bind(fd, host, port);
  if (!bind) {
    ::close(fd);
    return kl::Err(bind.MoveErr());
  }
  int err = ::listen(fd, 64);
  if (err != 0) {
    ::close(fd);
    return kl::Err(errno, "can't listen on port %u, %s", port,
                   std::strerror(errno));
  }
  return kl::Ok();
}

inline Status SetRecvTimeout(int fd, int timeout) {
  struct timeval tv;
  tv.tv_sec = timeout / 1000;
  tv.tv_usec = timeout % 1000;
  int err = ::setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
  if (err < 0) {
    return Err(errno, std::strerror(errno));
  }
  return kl::Ok();
}

inline Result<std::tuple<ssize_t, std::string, uint16_t>>
RecvFrom(int fd, void *buf, size_t size, int flags) {
  struct sockaddr_in from;
  socklen_t len = sizeof(from);
  ssize_t nread = ::recvfrom(fd, buf, size, flags,
                             reinterpret_cast<struct sockaddr *>(&from), &len);
  if (nread < 0) {
    return Err(errno, std::strerror(errno));
  }
  auto addr = InetAddr(from);
  return kl::Ok(
      std::make_tuple(nread, std::move(std::get<0>(addr)), std::get<1>(addr)));
}

inline Result<std::tuple<ssize_t, std::string, uint16_t>>
RecvFrom(int fd, void *buf, size_t size, int flags, int timeout) {
  auto settimeout = SetRecvTimeout(fd, timeout);
  if (!settimeout) {
    return Err(settimeout.MoveErr());
  }
  return RecvFrom(fd, buf, size, flags);
}

inline Result<ssize_t> Sendto(int fd, const void *buf, size_t size, int flags,
                              const struct sockaddr *addr) {
  socklen_t len = sizeof(*addr);
  ssize_t nwrite = ::sendto(fd, buf, size, flags, addr, len);
  if (nwrite < 0) {
    return Err(errno, std::strerror(errno));
  }
  return kl::Ok(nwrite);
}

inline Result<ssize_t> Sendto(int fd, const void *buf, size_t size, int flags,
                              const char *host, uint16_t port) {
  auto sockaddr = InetSockAddr(host, port);
  if (!sockaddr) {
    return Err(errno, std::strerror(errno));
  }
  return Sendto(fd, buf, size, flags,
                reinterpret_cast<const struct sockaddr *>(&(*sockaddr)));
}

inline int SocketError(int fd) {
  int error = 0;
  socklen_t len = sizeof(error);
  if (::getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len) == 0) {
    return error;
  }
  return error;
}

}  // namespace inet
}  // namespace kl
#endif
