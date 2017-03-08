// Copyright (c) 2016 Kai Luo. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_BUFFER_H_
#define KL_BUFFER_H_
#include <unistd.h>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <utility>
#include <vector>

#include "error.h"

namespace kl {

namespace {
inline size_t Align(size_t n) { return (n + 7) & (~0U << 3); }
}

class Buffer {
public:
  static size_t kMinimumSize;
  Buffer();

  explicit Buffer(size_t n);

  Buffer(const Buffer &b);
  Buffer(Buffer &&b);
  Buffer &operator=(Buffer &&b);

  void Reset() { r_ = w_ = 0; }
  size_t Len() const {
    assert(w_ >= r_);
    return w_ - r_;
  }

  size_t Cap() const { return cap_; }

  bool Empty() const { return r_ == w_; }

  size_t Avail() const {
    assert(cap_ >= w_);
    return cap_ - w_;
  }

  size_t Idle() const { return r_ + Avail(); }

  std::string Peek(size_t n);

  std::string PeekAll();

  Result<size_t> ReadFrom(int fd);

  size_t ReadFrom(const char *buf, size_t count);

  size_t ReadFrom(const std::vector<char> &buf) {
    return ReadFrom(buf.data(), buf.size());
  }

  size_t ReadFrom(const std::string &buf) {
    return ReadFrom(buf.data(), buf.size());
  }

  Result<size_t> WriteTo(int fd);

  const char *Data() const { return buf_; }

  ~Buffer() { delete[] buf_; }

protected:
  void MakeRoomForReadFrom(size_t count);

  void AlignToHead() {
    size_t len = Len();
    ::memmove(buf_, buf_ + r_, len);
    w_ = len;
    r_ = 0;
  }

  void ExtendTo(size_t estimated_size);

private:
  size_t cap_;
  char *buf_;
  size_t r_, w_;
};
}  // namespace kl

#endif
