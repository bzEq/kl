// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "coding.h"
#include "testkit.h"

namespace {
class T {};

TEST(T, GetBigEndianInteger) {
  kl::Slice s("\x01\x03", 2);
  auto x = kl::coding::GetBigEndianInteger<uint16_t>(s);
  ASSERT(x == 259);
}
}  // namespace
