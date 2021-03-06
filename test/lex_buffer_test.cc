// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "kl/lex_buffer.h"
#include "kl/logger.h"
#include "kl/slice.h"
#include "kl/testkit.h"

namespace {
TEST(kl::LexBuffer, SkipWhitespaces, "     wtf") {
  KL_DEBUG("buffer len: %u", Length());
  SkipWhitespaces();
  ASSERT(Next() == 'w');
  SkipWhitespaces();
  ASSERT(Next() == 't');
  SkipWhitespaces();
  ASSERT(Next() == 'f');
}

TEST(kl::LexBuffer, LookAhead, "  wtf") {
  ASSERT(Expect("  "));
  KL_DEBUG("buffer len: %u", Length());
  ASSERT(!Expect("wtf"));
  SkipWhitespaces();
  ASSERT(Expect("wtf"));
}

TEST(kl::LexBuffer, Skip, "wtfrofl") {
  Skip(Length());
  ASSERT(!HasNext());
  ASSERT(!Expect('w'));
  ASSERT(!Expect("wtf"));
}

TEST(kl::LexBuffer, Next, "\x02\x01") {
  ASSERT(Length() == 2);
  Skip(1);
  auto next = Next<uint8_t>();
  ASSERT(next);
  ASSERT(*next == 1);
  ASSERT(!HasNext());
}

TEST(kl::LexBuffer, BigEndianNext, "\x01\x01") {
  ASSERT(Length() == 2);
  auto next = BigEndianNext<uint16_t>();
  ASSERT(next);
  ASSERT(*next == 257);
  ASSERT(Remaining() == 0);
}

TEST(kl::LexBuffer, Next1, "wtf") {
  Skip(1);
  kl::Slice s(nullptr, 2);
  Next(&s);
  ASSERT(s.data);
  ASSERT(std::string(s.data, s.len) == "tf");
  ASSERT(!HasNext());
}
}  // namespace
