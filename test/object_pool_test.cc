// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.
#include <iostream>
#include <string>

#include "kl/logger.h"
#include "kl/object_pool.h"
#include "kl/testkit.h"

namespace {
TEST(kl::ObjectPool<std::string>, New) {
  for (int i = 0; i < 1024; ++i) {
    std::string *p = New("wtf");
    ASSERT(*p == "wtf");
  }
  for (int i = 0; i < 1024; ++i) {
    size_t index = NewAt("wtf");
    ASSERT(*At(index) == "wtf");
  }
  ASSERT(!Empty());
  ASSERT(Size() == 2048);
}

TEST(kl::ObjectPool<uint8_t>, NewArray) {
  auto a = NewArray(1024);
  ASSERT(Size() == 1);
  for (int i = 0; i < 1024; ++i) {
    ASSERT(a[i] == 0);
  }
}

}  // namespace
