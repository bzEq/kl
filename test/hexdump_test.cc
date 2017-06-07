// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "kl/hexdump.h"
#include "kl/testkit.h"

#include <iostream>

namespace {

class HexDumpTest {};

TEST(HexDumpTest, SimpleDump) {
  const char s[4] = {static_cast<char>(0xca), static_cast<char>(0xfe),
                     static_cast<char>(0xba), static_cast<char>(0xbe)};
  kl::Slice slice(s, sizeof(s));
  std::string result;
  kl::HexDump(slice, &result);
  std::cerr << result << "\n";
  ASSERT(result == "ca fe ba be");
}

}  // namespace
