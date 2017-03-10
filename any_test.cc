// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include <iostream>
#include <string>

#include "any.h"
#include "testkit.h"

class T {};

TEST(T, Any) {
  kl::Any any(std::string("wtf"));
  auto store = any.Of<std::string>();
  ASSERT(store);
}

TEST(T, Any1) {
  kl::Any any(std::string("wtf"));
  auto store = any.Of<int>();
  ASSERT(!store);
}

int main() { return KL_TEST(); }
