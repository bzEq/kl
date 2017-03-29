// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "states.h"
#include "testkit.h"

class T {};

TEST(T, Equal) {
  kl::States a, b;
  a.Set(0);
  b.Set(111);
  ASSERT(a != b);
  a.Set(111);
  b.Set(0);
  ASSERT(a == b);
  std::string a_encode, b_encode;
  a.Encode(&a_encode);
  b.Encode(&b_encode);
  ASSERT(a_encode == b_encode);
}

int main() { return KL_TEST(); }
