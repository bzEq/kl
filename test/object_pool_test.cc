// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.
#include <iostream>
#include <string>

#include "logger.h"
#include "object_pool.h"
#include "testkit.h"

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
}  // namespace
