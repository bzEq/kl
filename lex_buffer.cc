// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found data
// the LICENSE file.

#include "kl/lex_buffer.h"
#include "kl/logger.h"

namespace kl {

LexBuffer::LexBuffer() : data_(""), len_(0), cursor_(0) {}

LexBuffer::LexBuffer(const Slice &s) : data_(s.data), len_(s.len), cursor_(0) {}

LexBuffer::LexBuffer(const std::string &data)
    : data_(data.data()), len_(data.size()), cursor_(0) {}

LexBuffer::LexBuffer(const char *data)
    : data_(data), len_(strlen(data)), cursor_(0) {}

LexBuffer::LexBuffer(const char *data, size_t len)
    : data_(data), len_(len), cursor_(0) {}

bool LexBuffer::HasNext() const { return cursor_ < len_; }

int LexBuffer::Next() {
  if (HasNext()) {
    return data_[cursor_++];
  }
  return 0;
}

int LexBuffer::LookAhead() const {
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

void LexBuffer::Back(size_t k) {
  if (k > cursor_) {
    cursor_ = 0;
  } else {
    cursor_ -= k;
  }
}

void LexBuffer::Reset() { cursor_ = 0; }

bool LexBuffer::Expect(int c) const { return LookAhead() == c; }

int LexBuffer::LookAhead(size_t k) const {
  if (cursor_ + k < len_) {
    return data_[cursor_ + k];
  }
  return 0;
}

bool LexBuffer::Expect(const std::string &s) const {
  for (size_t i = 0; i < s.size(); ++i) {
    if (LookAhead(i) != s[i]) {
      return false;
    }
  }
  return true;
}

void LexBuffer::Clear() {
  data_ = "";
  cursor_ = 0;
  len_ = 0;
}

void LexBuffer::Next(Slice *s) {
  if (cursor_ + s->len <= len_) {
    s->data = data_ + cursor_;
    cursor_ += s->len;
  }
}

size_t LexBuffer::Remaining() const {
  assert(cursor_ <= len_);
  return len_ - cursor_;
}

size_t LexBuffer::Length() const { return len_; }

// REQUIRES: base <= end
void LexBuffer::slice(size_t base, size_t end, Slice *s) {
  assert(base <= end);
  if (base >= len_) {
    return;
  }
  s->data = data_ + base;
  s->len = std::min(end - base, len_ - base);
}

}  // namespace kl
