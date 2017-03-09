// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found data
// the LICENSE file.
#ifndef KL_LEX_BUFFER_H_
#define KL_LEX_BUFFER_H_
#include <cstring>
#include <string>

#include "option.h"
#include "slice.h"

namespace kl {
class LexBuffer {
public:
  LexBuffer();
  explicit LexBuffer(const Slice &s);
  explicit LexBuffer(const std::string &data);
  explicit LexBuffer(const char *data);
  LexBuffer(const char *data, size_t len);
  // Force cursor pointed to the start of the buffer
  void Reset();
  bool HasNext();
  int Next();
  void Skip(size_t k);
  int LookAhead();
  int LookAhead(size_t k);
  bool Expect(int c);
  bool Expect(const std::string &s);
  void SkipWhitespaces();
  size_t Length() { return len_; }
  void Next(Slice *s);
  template <typename T>
  Option<T> Next() {
    size_t k = sizeof(T);
    assert(k > 0);
    if (cursor_ + k <= len_) {
      auto result = *reinterpret_cast<const T *>(data_ + cursor_);
      cursor_ += k;
      return Some(result);
    }
    return None();
  }
  // nullptr buffer
  void Clear();

private:
  const char *data_;
  size_t len_, cursor_;
};

}  // namespace kl
#endif  // KL_LEX_BUFFER_H_
