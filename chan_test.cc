// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#include <iostream>
#include <memory>
#include <thread>

#include "chan.h"
#include "testkit.h"

class C {};

TEST(C, PushAndPop) {
  auto ch = std::make_shared<kl::Chan<int>>();
  auto t = std::thread([ch]() {
    *ch << 17;
    ch->Push(19);
    ch->Close();
  });
  auto pop = ch->Pop();
  ASSERT(pop);
  ASSERT(*pop == 17);
  pop = ch->Pop();
  ASSERT(*pop == 19);
  t.join();
  ASSERT(ch->Closed());
}

int main() { return KL_TEST(); }
