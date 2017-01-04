// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#include <iostream>

#include "netdev.h"
#include "testkit.h"

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

int main() { return KL_TEST(); }
