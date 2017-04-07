// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.
#include <string>

#include "testkit.h"
#include "utf8.h"

namespace {
class T {};
}

TEST(T, PrefixWith) {
  ASSERT(kl::utf8::PrefixWith(0xc0, 0x03));
  ASSERT(kl::utf8::PrefixWith(0xa0, 0xa));
  ASSERT(kl::utf8::PrefixWith(0x81, 0x1));
  ASSERT(kl::utf8::PrefixWith(0x81, 0x2));
  ASSERT(kl::utf8::PrefixWith(0x81, 0x4));
  ASSERT(kl::utf8::PrefixWith(0x90, 0x1));
  ASSERT(!kl::utf8::PrefixWith(0x70, 0x70));
}

TEST(kl::utf8::Iterator, Iterate, std::string("$")) {
  kl::Slice s;
  Next(&s);
  ASSERT(s.data);
  ASSERT(s.len == 1);
}
