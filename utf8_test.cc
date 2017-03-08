// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "utf8.h"
#include "testkit.h"

class T {};

TEST(T, PrefixWith) {
  ASSERT(kl::utf8::PrefixWith(0xc0, 0x03));
  ASSERT(kl::utf8::PrefixWith(0xa0, 0xa));
  ASSERT(kl::utf8::PrefixWith(0x81, 0x1));
  ASSERT(kl::utf8::PrefixWith(0x81, 0x2));
  ASSERT(kl::utf8::PrefixWith(0x81, 0x4));
  ASSERT(kl::utf8::PrefixWith(0x90, 0x1));
  ASSERT(!kl::utf8::PrefixWith(0x70, 0x70));
}

TEST(kl::utf8::Iterator, Codepoint, "$") {
  auto next = Next();
  ASSERT(next);
  ASSERT(next->len == 1);
}

TEST(kl::utf8::Iterator, Codepoint1, "¢€𐍈") {
  auto next = Next();
  ASSERT(next);
  ASSERT(next->len == 2);
  next = Next();
  ASSERT(next);
  ASSERT(next->len == 3);
  next = Next();
  ASSERT(next);
  ASSERT(next->len == 4);
}

int main() { return KL_TEST(); }
