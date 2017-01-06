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
#include "inet.h"

namespace kl {
namespace tcp {

inline Result<int> Socket() {
  int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
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
  auto listen = kl::inet::Listen(*sock, host, port);
  if (!listen) {
    ::close(*sock);
    return kl::Err(listen.MoveErr());
  }
  return kl::Ok(*sock);
}

inline Result<int> BlockingConnect(const char *host, uint16_t port) {
  auto sock = Socket();
  if (!sock) {
    return kl::Err(sock.MoveErr());
  }
  int fd = *sock;
  auto addr = kl::inet::InetSockAddr(host, port);
  int err = ::connect(fd, reinterpret_cast<const struct sockaddr *>(&(*addr)),
                      sizeof(*addr));
  if (err < 0) {
    ::close(fd);
    return kl::Err(errno, std::strerror(errno));
  }
  return kl::Ok(fd);
}

}  // namespace tcp
}  // namespace kl
#endif
