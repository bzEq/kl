// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#include <iostream>

#include "netdev.h"
#include "testkit.h"

class N {};
TEST(N, ListInterface) {
  auto list = kl::netdev::ListInterface();
  ASSERT(list);
  ASSERT((*list).size());
  for (const auto &ifreq : *list) {
    const char *ifname = ifreq.ifr_name;
    const char *addr =
        inet_ntoa(reinterpret_cast<const struct sockaddr_in *>(&ifreq.ifr_addr)
                      ->sin_addr);
    std::cout << ifname << ": " << addr << std::endl;
  }
}

int main() { return KL_TEST(); }
