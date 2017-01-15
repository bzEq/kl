// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_UDP_H_
#define KL_UDP_H_
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>

#include "error.h"
#include "inet.h"

namespace kl {
namespace udp {

inline Result<int> Socket() {
  int fd = ::socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    return kl::Err(errno, std::strerror(errno));
  }
  return kl::Ok(fd);
}

}  // udp
}  // namespace kl
#endif
