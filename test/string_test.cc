// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "string.h"
#include "testkit.h"

namespace {
class T {};

TEST(T, SplitString) {
  const char *ip = "127.0.0.1";
  auto result = kl::string::SplitString(ip, ".");
  ASSERT(result.size() == 4);
  ASSERT(result[0] == "127");
  ASSERT(result[1] == "0");
  ASSERT(result[2] == "0");
  ASSERT(result[3] == "1");
}

TEST(T, SplitString1) {
  const char *path = "kl::string::SplitString";
  auto result = kl::string::SplitString(path, "::");
  ASSERT(result.size() == 3);
  ASSERT(result[0] == "kl");
  ASSERT(result[1] == "string");
  ASSERT(result[2] == "SplitString");
}
}  // namespace
