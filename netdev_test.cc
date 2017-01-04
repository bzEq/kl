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
    std::cout << ifreq.ifr_name << std::endl;
  }
}

int main() { return KL_TEST(); }
