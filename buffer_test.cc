// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "buffer.h"
#include "testkit.h"

TEST(kl::Buffer, Constructor, 1024) { ASSERT(Cap() == 1024); }

TEST(kl::Buffer, ExtendTo) {
  size_t cap = Cap();
  ExtendTo(2 * cap);
  ASSERT(Cap() == 2 * cap);
  ExtendTo(cap);
  ASSERT(Cap() == 2 * cap);
}

TEST(kl::Buffer, Idle) {
  size_t cap = Cap();
  ReadFrom("wtf", 3);
  auto s = Peek(1);
  ASSERT(s == "w");
  ASSERT(Len() == 2);
  ASSERT(Cap() == cap);
  ASSERT(1 + Avail() == Idle());
}

int main() { return KL_TEST(); }
