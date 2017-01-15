// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include <string>

#include "hash.h"
#include "testkit.h"

class H {};

TEST(H, CompareEqual) {
  std::string data("こんにちは");
  uint16_t hash0 = kl::hash::Fletcher16(data.c_str(), data.size());
  uint16_t hash1 = kl::hash::Fletcher16(data.c_str(), data.size());
  ASSERT(hash0 == hash1);
}

TEST(H, CompareUnequal) {
  std::string jp("こんにちは");
  std::string en("hello");
  uint16_t hash0 = kl::hash::Fletcher16(jp.c_str(), jp.size());
  uint16_t hash1 = kl::hash::Fletcher16(en.c_str(), en.size());
  ASSERT(hash0 != hash1);
}

int main() { return KL_TEST(); }
