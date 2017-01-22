// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.
#include <algorithm>
#include <random>

#include "heap.h"
#include "logger.h"
#include "testkit.h"
#include "timer.h"

TEST(kl::Heap<uint32_t>, RandomInsert) {
  static const size_t kMaxSize = 1 << 20;
  std::vector<uint32_t> sample;
  for (size_t i = 0; i < kMaxSize; ++i) {
    sample.push_back(i);
  }
  std::random_shuffle(sample.begin(), sample.end());
  for (size_t i = 0; i < kMaxSize; ++i) {
    Insert(sample[i]);
  }
  std::vector<uint32_t> result;
  std::chrono::duration<float> cost;
  kl::timer::FunctionCost(&cost, std::function<void(void)>([this, &result] {
    while (!Empty()) {
      result.push_back(std::move(Top()));
      Pop();
    }
  }));
  ASSERT(Empty());
  ASSERT(std::is_sorted(result.begin(), result.end()));
  KL_DEBUG("cost %fs", cost.count());
}

int main() { KL_TEST(); }
