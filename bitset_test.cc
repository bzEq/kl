// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#include <iostream>

#include "bitset.h"
#include "testkit.h"

TEST(kl::BitSet, C0, 0) { ASSERT(Size() == 8); }
TEST(kl::BitSet, C1, 1) { ASSERT(Size() == 8); }
TEST(kl::BitSet, C2, 2) { ASSERT(Size() == 8); }
TEST(kl::BitSet, C3, 3) { ASSERT(Size() == 8); }
TEST(kl::BitSet, C4, 4) { ASSERT(Size() == 8); }
TEST(kl::BitSet, C5, 5) { ASSERT(Size() == 8); }
TEST(kl::BitSet, C6, 6) { ASSERT(Size() == 8); }
TEST(kl::BitSet, C7, 7) { ASSERT(Size() == 8); }
TEST(kl::BitSet, C8, 8) { ASSERT(Size() == 8); }
TEST(kl::BitSet, C9, 9) { ASSERT(Size() == 16); }
TEST(kl::BitSet, C10, 10) { ASSERT(Size() == 16); }

static const size_t num_of_bits = 1024;

TEST(kl::BitSet, SetAndTest, num_of_bits) {
  for (int i = 0; i < 128; ++i) {
    Set(i);
    ASSERT(Test(i));
  }
}

TEST(kl::BitSet, SetAndClear, num_of_bits) {
  for (int i = 0; i < 128; ++i) {
    Set(i);
    ASSERT(Test(i));
    Clear(i);
    ASSERT(!Test(i));
  }
}

TEST(kl::BitSet, FirstZeroBit, 128) {
  Set(7);
  int i = SetFirstZeroBit();
  ASSERT(i == 0);
  ASSERT(Test(i));
  Set(1);
  ASSERT(Test(1));
  ASSERT(!Test(2));
  i = SetFirstZeroBit();
  ASSERT(i == 2);
}

int main() {
  KL_TEST();
  return 0;
}
