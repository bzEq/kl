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

// RETURN: fd
inline Result<int> Listen(const char *host, uint16_t port) {
  int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    return kl::Err(errno, std::strerror(errno));
  }
  struct sockaddr_in addr = {
      .sin_family = AF_INET, .sin_port = htons(port),
  };
  int err = ::inet_aton(host, &addr.sin_addr);
  if (!err) {
    ::close(fd);
    return kl::Err("invalid ip address: %s", host);
  }
  err = ::bind(fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr));
  if (err != 0) {
    ::close(fd);
    return kl::Err(errno, "can't bind the socket on port %u, %s", port,
                   std::strerror(errno));
  }
  err = ::listen(fd, 64);
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
