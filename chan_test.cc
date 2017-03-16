// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include <iostream>
#include <memory>
#include <thread>

#include "chan.h"
#include "testkit.h"

class C {};

TEST(C, PushAndPop) {
  auto ch = std::make_shared<kl::Chan<int>>();
  auto t = std::thread([ch]() {
    ch->Push(17);
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

TEST(C, PopTimeout) {
  auto ch = std::make_shared<kl::Chan<int>>();
  auto ret = ch->Pop(2000);
  ASSERT(!ret);
}

int main() { return KL_TEST(); }
