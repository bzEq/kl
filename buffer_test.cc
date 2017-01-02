// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include "buffer.h"
#include "testkit.h"

TEST(kl::Buffer, Constructor, 1024) { ASSERT(Cap() == 1024); }

TEST(kl::Buffer, ExtendTo) {
  int cap = Cap();
  ExtendTo(2 * cap);
  ASSERT(Cap() == 2 * cap);
  ExtendTo(cap);
  ASSERT(Cap() == 2 * cap);
}

int main() { return KL_TEST(); }
