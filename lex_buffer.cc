// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found data
// the LICENSE file.
#include "lex_buffer.h"
#include "logger.h"

namespace kl {

LexBuffer::LexBuffer() : data_(nullptr), len_(0), cursor_(0) {}

LexBuffer::LexBuffer(const Slice &s) : data_(s.data), len_(s.len), cursor_(0) {}

LexBuffer::LexBuffer(const std::string &data)
    : data_(data.data()), len_(data.size()), cursor_(0) {}

LexBuffer::LexBuffer(const char *data)
    : data_(data), len_(strlen(data)), cursor_(0) {}

LexBuffer::LexBuffer(const char *data, size_t len)
    : data_(data), len_(len), cursor_(0) {}

bool LexBuffer::HasNext() { return cursor_ < len_; }

int LexBuffer::Next() {
  if (HasNext()) {
    return data_[cursor_++];
  }
  return 0;
}

int LexBuffer::LookAhead() {
  if (HasNext()) {
    return data_[cursor_];
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
    return data_[cursor_ + k];
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
  data_ = nullptr;
  cursor_ = 0;
  len_ = 0;
}

void LexBuffer::Next(Slice *s) {
  if (cursor_ + s->len <= len_) {
    s->data = data_ + cursor_;
    cursor_ += s->len;
  }
}

}  // namespace kl
