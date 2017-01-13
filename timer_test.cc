// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
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
  int sum = kl::timer::FunctionCost(&diff, Sum, 100);
  ASSERT(sum == 5050);
  KL_DEBUG("Sum costs %fs", diff.count());
}

TEST(T, SumUp) {
  std::chrono::duration<float> diff;
  T test;
  int sum = kl::timer::FunctionCost(&diff, &T::SumUp, &test, 100);
  ASSERT(sum == 5050);
  KL_DEBUG("SumUp costs %fs", diff.count());
}

int main() { return KL_TEST(); }