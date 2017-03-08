// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "utf8.h"

namespace kl {
namespace utf8 {

// CAUTIOUS: It's *not* an universal routine.
// REQUIRES: prefix > 0
bool PrefixWith(uint8_t byte, uint8_t prefix) {
  assert(prefix > 0);
  uint8_t shift = __builtin_clz(prefix) - 24;
  return !((byte & (0xff << shift)) ^ (prefix << shift));
}

uint8_t Prefix(uint8_t x, uint8_t bits_of_prefix) {
  return x & ~(0xff >> bits_of_prefix);
}

bool PrefixEqual(uint8_t x, uint8_t y, uint8_t bits_of_prefix) {
  return !(Prefix(x, bits_of_prefix) ^ Prefix(y, bits_of_prefix));
}

Iterator::Iterator(const std::string &s) : buffer_(s) {}

Iterator::Iterator(const Slice &s) : buffer_(s) {}

Iterator::Iterator(const char *s, size_t len) : buffer_(s, len) {}

// 4 bitfield patterns
// 0xxxxxxx
// 110xxxxx 10xxxxxx
// 1110xxxx 10xxxxxx 10xxxxxx
// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
// 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
// 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
Option<Slice> Iterator::Next() {
  if (!buffer_.HasNext()) {
    return None();
  }
  Slice s(nullptr, 1);
  buffer_.Next(&s);
  assert(s.data != nullptr);
  uint8_t fst = static_cast<uint8_t>(s.data[0]);
  if ((fst & (0x80)) == 0) {
    return s;
  }
  for (uint8_t k = 3; k <= 7; ++k) {
    if (PrefixEqual(fst, 0xff << (9 - k), k)) {
      for (uint8_t i = 0; i < k - 2; ++i) {
        if (!buffer_.HasNext()) {
          return None();
        }
        uint8_t sub = static_cast<uint8_t>(buffer_.Next());
        if (!PrefixWith(sub, 0x2)) {
          return None();
        }
        ++s.len;
      }
      return s;
    }
  }
  return None();
}

}  // namespace utf8
}  // namespace kl
