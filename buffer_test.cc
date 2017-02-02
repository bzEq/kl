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

int main() { return KL_TEST(); }
