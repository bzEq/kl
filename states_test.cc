// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "states.h"
#include "testkit.h"

class T {};

TEST(T, Equal) {
  kl::States a;
  kl::States b;
  a.Set(0);
  b.Set(111);
  ASSERT(a != b);
  a.Set(111);
  b.Set(0);
  ASSERT(a == b);
}

int main() { return KL_TEST(); }
