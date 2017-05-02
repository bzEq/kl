// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include <iostream>
#include <string>

#include "kl/any.h"
#include "kl/testkit.h"

namespace {
class T {};

TEST(T, Any) {
  const std::string str("wtf");
  kl::Any any(str);
  auto store = any.IsOf<std::string>();
  ASSERT(store);
}

TEST(T, Any1) {
  kl::Any any(std::string("wtf"));
  auto store = any.IsOf<int>();
  ASSERT(!store);
}

TEST(T, Any2) {
  kl::Any any;
  any = 1;
  ASSERT(any.IsOf<int>());
  any = std::string("wtf");
  ASSERT(!any.IsOf<int>());
  ASSERT(any.IsOf<std::string>());
}

TEST(T, Any3) {
  kl::Any a, b;
  a = static_cast<uint64_t>(0U);
  b = std::string("wtf");
  std::swap(a, b);
  ASSERT(a.IsOf<std::string>());
  ASSERT(!a.Of<uint64_t>());
  ASSERT(b.IsOf<uint64_t>());
  ASSERT(!b.Of<std::string>());
}
}  // namespace
