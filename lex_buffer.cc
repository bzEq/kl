// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.
#include "lex_buffer.h"

namespace kl {

LexBuffer::LexBuffer() : in_(nullptr), cursor_(0), len_(0) {}

LexBuffer::LexBuffer(const Slice &s) : in_(s.data), cursor_(0), len_(s.len) {}

LexBuffer::LexBuffer(const std::string &in)
    : in_(in.data()), cursor_(0), len_(in.size()) {}

LexBuffer::LexBuffer(const char *in, size_t len)
    : in_(in), cursor_(0), len_(len) {}

bool LexBuffer::HasNext() { return cursor_ < len_; }

int LexBuffer::Next() {
  if (HasNext()) {
    return in_[cursor_++];
  }
  return 0;
}

int LexBuffer::LookAhead() {
  if (HasNext()) {
    return in_[cursor_];
  }
  return 0;
}

void LexBuffer::SkipWhitespaces() {
  while (HasNext() && std::isspace(LookAhead())) {
    Next();
  }
}

void LexBuffer::Skip(size_t k) { cursor_ = std::min(cursor_ + k, len_); }

void LexBuffer::Reset() {}

bool LexBuffer::Expect(int c) { return LookAhead() == c; }

int LexBuffer::LookAhead(size_t k) {
  if (cursor_ + k < len_) {
    return in_[cursor_ + k];
  }
  return 0;
}

bool LexBuffer::Expect(const std::string &s) {
  for (size_t i = 0; i < s.size(); ++i) {
    if (LookAhead(i) != s[i]) {
      return false;
    }
  }
  return true;
}

void LexBuffer::Clear() {
  in_ = nullptr;
  cursor_ = 0;
  len_ = 0;
}

void LexBuffer::Next(Slice *s) {
  if (cursor_ + s->len <= len_) {
    s->data = in_ + cursor_;
    cursor_ += s->len;
  }
}

}  // namespace kl
