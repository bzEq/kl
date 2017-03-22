// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include <chrono>

#include "logger.h"
#include "testkit.h"
#include "timer.h"

static int Sum(int n) {
  int sum = 0;
  for (int i = 1; i <= n; ++i) {
    sum += i;
  }
  return sum;
}

class T {
public:
  int SumUp(int n) { return Sum(n); }
};

TEST(T, Sum) {
  std::chrono::duration<float> diff;
  int sum = kl::timer::FunctionCost(&diff, &Sum, 100);
  ASSERT(sum == 5050);
  KL_DEBUG("Sum costs %fs", diff.count());
}

TEST(T, Sum1) {
  std::chrono::duration<float> diff;
  kl::timer::FunctionCost(&diff, Sum, 100);
}

TEST(T, SumUp) {
  std::chrono::duration<float> diff;
  T test;
  int sum = kl::timer::FunctionCost(&diff, [&test] { return test.SumUp(100); });
  ASSERT(sum == 5050);
  KL_DEBUG("SumUp costs %fs", diff.count());
}

int main() { return KL_TEST(); }
