// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#ifndef KL_TCP_H_
#define KL_TCP_H_
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>

#include "error.h"

namespace kl {
namespace tcp {

inline Result<int> Socket() {
  int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    return kl::Err(errno, std::strerror(errno));
  }
  return kl::Ok(fd);
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

inline Result<void> Bind(int fd, const char *host, uint16_t port) {
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

inline Result<int> BlockingConnect(const char *host, uint16_t port) {
  auto sock = Socket();
  if (!sock) {
    return kl::Err(sock.MoveErr());
  }
  int fd = *sock;
  auto addr = InetSockAddr(host, port);
  int err = ::connect(fd, reinterpret_cast<const struct sockaddr *>(&(*addr)),
                      sizeof(*addr));
  if (err < 0) {
    ::close(fd);
    return kl::Err(errno, std::strerror(errno));
  }
  return kl::Ok(fd);
}

// RETURN: fd
inline Result<int> Listen(const char *host, uint16_t port) {
  auto sock = Socket();
  if (!sock) {
    return kl::Err(sock.MoveErr());
  }
  int fd = *sock;
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
  return kl::Ok(fd);
}

}  // namespace tcp
}  // namespace kl
#endif
