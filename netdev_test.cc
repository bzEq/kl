// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#include <iostream>

#include "netdev.h"
#include "testkit.h"

class N {};
TEST(N, ListInterfaces) {
  auto list = kl::netdev::ListInterfaces();
  ASSERT(list);
  ASSERT((*list).size());
}

TEST(N, PrintInterfaces) { ASSERT(kl::netdev::PrintInterfaces(std::cout)); }

int main() { return KL_TEST(); }
