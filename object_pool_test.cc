// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.
#include <iostream>
#include <string>

#include "logger.h"
#include "object_pool.h"
#include "testkit.h"

TEST(kl::ObjectPool<std::string>, Allocate) {
  for (int i = 0; i < 1024; ++i) {
    std::string *p = Allocate("wtf");
    ASSERT(*p == "wtf");
  }
}

int main() { return KL_TEST(); }
