// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

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

static const size_t kNumOfBits = 1023;

TEST(kl::BitSet, SetAndTest, kNumOfBits) {
  for (size_t i = 0; i < Size(); ++i) {
    Set(i);
    ASSERT(Test(i));
  }
}

TEST(kl::BitSet, SetAndClear, kNumOfBits) {
  for (size_t i = 0; i < Size(); ++i) {
    Set(i);
    ASSERT(Test(i));
    Clear(i);
    ASSERT(!Test(i));
  }
}

TEST(kl::BitSet, FirstZeroBit, kNumOfBits) {
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

TEST(kl::BitSet, NoMoreZeroBit, kNumOfBits) {
  for (size_t i = 0; i < Size(); ++i) {
    Set(i);
  }
  int i = SetFirstZeroBit();
  ASSERT(i == -1);
}
