// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_UTF8_H_
#define KL_UTF8_H_
#include <string>

#include "kl/error.h"
#include "kl/lex_buffer.h"
#include "kl/logger.h"
#include "kl/option.h"
#include "kl/slice.h"

namespace kl {
namespace utf8 {

bool PrefixWith(uint8_t byte, uint8_t prefix);

class Iterator {
public:
  explicit Iterator(const std::string &s);
  explicit Iterator(const Slice &s);
  explicit Iterator(const char *s);
  Iterator(const char *s, size_t len);
  // return next utf8 character
  void Next(Slice *s);

private:
  LexBuffer buffer_;
};

}  // utf8
}  // namespace kl
#endif  // KL_UTF8_H_
