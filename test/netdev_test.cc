// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include <iostream>

#include "kl/netdev.h"
#include "kl/testkit.h"

namespace {
class N {};

TEST(N, ListIPv4Interfaces) {
  auto list = kl::netdev::ListIPv4Interfaces();
  ASSERT(list);
  ASSERT((*list).size());
}

TEST(N, PrintIPv4Interfaces) {
  ASSERT(kl::netdev::PrintIPv4Interfaces(std::cout));
}

TEST(N, RetrieveIFIndex) {
  auto index = kl::netdev::RetrieveIFIndex("lo");
  ASSERT(index);
  auto name = kl::netdev::RetrieveIFName(*index);
  ASSERT(name);
  ASSERT(*name == "lo");
}

TEST(N, FailRetrieveIFIndex) {
  auto index = kl::netdev::RetrieveIFIndex("lol");
  ASSERT(!index);
}

TEST(N, GetAddress) {
  auto addr = kl::netdev::GetAddr("lo");
  ASSERT(addr);
  ASSERT(*addr == "127.0.0.1");
}

TEST(N, GetMTU) {
  auto list = kl::netdev::ListIPv4Interfaces();
  ASSERT(list);
  for (const auto &ifr : *list) {
    auto mtu = kl::netdev::GetMTU(ifr.ifr_name);
    ASSERT(mtu);
    ASSERT(*mtu >= 0);
    std::cout << ifr.ifr_name << "'s mtu: " << *mtu << "\n";
  }
}
}  // namespace
