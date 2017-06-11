// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "kl/static_value.h"
#include "kl/testkit.h"

#include <string>

namespace {

class StaticValueTest {
public:
  std::string Name() const { return "StaticValueTest"; }
};

kl::StaticValue<StaticValueTest> instance;

TEST(StaticValueTest, Basic) {
  auto &test = *instance;
  ASSERT(test.Name() == "StaticValueTest");
}

} // namespace
