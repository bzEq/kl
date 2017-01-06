// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#include "inet.h"
#include "testkit.h"

class T {};

TEST(T, InetSockAddr) {
  auto addr0 = kl::inet::InetSockAddr("192.168.1.1", 3000);
  ASSERT(addr0);
  auto addr1 = kl::inet::InetSockAddr("192.168.1.257", 2000);
  ASSERT(!addr1);
}

int main() { return KL_TEST(); }
