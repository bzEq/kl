// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include <string>

#include "memory_pool.h"
#include "testkit.h"

namespace {

class T {};

TEST(T, Allocate) {
  kl::Allocator allocator;
  auto p = allocator.Allocate<std::string>("wtf");
  ASSERT(*p == "wtf");
}

TEST(T, AllocateArray) {
  kl::Allocator allocator;
  auto p = allocator.AllocateArray<uint8_t>(4096);
  for (int i = 0; i < 4096; ++i) {
    ASSERT(p[i] == 0);
  }
}

}  // namespace
