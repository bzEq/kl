// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include <memory>
#include <vector>

#include "testkit.h"

namespace kl {
namespace test {

namespace {
struct TestCase {
  const char *base;
  const char *name;
  void (*func)();
};
std::unique_ptr<std::vector<TestCase>> tests;
}

bool RegisterTest(const char *base, const char *name, void (*func)(void)) {
  if (!tests) {
    tests = std::make_unique<std::vector<TestCase>>();
  }
  tests->push_back(TestCase{
      .base = base, .name = name, .func = func,
  });
  return true;
}

int RunAllTests() {
  size_t passed = 0;
  if (tests) {
    for (size_t i = 0; i < tests->size(); ++i) {
      const auto &t = (*tests)[i];
      std::fprintf(stderr, "==== Test %s.%s\n", t.base, t.name);
      try {
        (*t.func)();
        ++passed;
      } catch (const std::exception &e) {
        std::fprintf(stderr, "==== Test %s.%s exception caught, %s\n", t.base,
                     t.name, e.what());
      }
    }
  }
  std::fprintf(stderr, "==== %lu/%lu TESTS PASSED\n", passed, tests->size());
  if (passed != tests->size()) {
    return 1;
  }
  return 0;
}

}  // namespace test
}  // namespace kl
