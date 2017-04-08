// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include <atomic>
#include <thread>

#include "testkit.h"
#include "wait_group.h"

namespace {
TEST(kl::WaitGroup, Sum) {
  std::atomic<int> sum(0);
  for (int i = 1; i <= 100; ++i) {
    Add();
    std::thread([&sum, this, i]() {
      sum += i;
      this->Done();
    }).detach();
  }
  Wait();
  ASSERT(sum == 5050);
}
}  // namespace
