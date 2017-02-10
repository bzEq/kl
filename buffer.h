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

class Buffer {
public:
  static constexpr size_t kMinimumSize = 16;
  Buffer() : cap_(kMinimumSize), r_(0), w_(0) { buf_ = new char[cap_]; }

  explicit Buffer(size_t n) : cap_(std::max(kMinimumSize, n)), r_(0), w_(0) {
    buf_ = new char[cap_];
  }

  Buffer(const Buffer &b) {
    if (cap_ < b.cap_) {
      delete[] buf_;
      cap_ = b.cap_;
      buf_ = new char[cap_];
    }
    std::memcpy(buf_, b.buf_, b.cap_);
    r_ = b.r_;
    w_ = b.w_;
  }

  Buffer(Buffer &&b) {
    std::swap(cap_, b.cap_);
    std::swap(buf_, b.buf_);
    std::swap(r_, b.r_);
    std::swap(w_, b.w_);
  }

  Buffer &operator=(Buffer &&b) {
    std::swap(cap_, b.cap_);
    std::swap(buf_, b.buf_);
    std::swap(r_, b.r_);
    std::swap(w_, b.w_);
    return *this;
  }

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

  std::string Peek(size_t n) {
    assert(n > 0);
    n = std::min(n, Len());
    std::string result;
    result.resize(n);
    std::memcpy(&result[0], buf_ + r_, n);
    r_ += n;
    return result;
  }

  std::string PeekAll() {
    size_t n = Len();
    std::string result;
    if (n == 0) {
      return result;
    }
    result = Peek(n);
    Reset();
    return result;
  }

  Result<size_t> ReadFrom(int fd) {
    assert(Avail() > 0);
    size_t m = 0;
    int n = ::read(fd, buf_ + w_, Avail());
    while (n > 0) {
      w_ += n;
      m += n;
      if (Avail() == 0) {
        if (Idle() > 0) {
          AlignToHead();
        } else {
          ExtendTo(2 * cap_);
        }
      }
      n = ::read(fd, buf_ + w_, Avail());
    }
    if (n < 0) {
      return Err(std::strerror(errno));
    }
    return Ok(m);
  }

  size_t ReadFrom(const char *buf, size_t count) {
    if (Avail() < count) {
      if (Idle() >= count) {
        AlignToHead();
      } else {
        ExtendTo(w_ + count);
      }
    }
    size_t n = std::min(count, Avail());
    std::memcpy(buf_ + w_, buf, n);
    w_ += n;
    return n;
  }

  size_t ReadFrom(const std::vector<char> &buf) {
    return ReadFrom(buf.data(), buf.size());
  }

  Result<size_t> WriteTo(int fd) {
    if (Len() == 0) {
      return Ok(Len());
    }
    size_t m = 0;
    int n = ::write(fd, buf_ + r_, Len());
    while (n > 0) {
      r_ += n;
      m += n;
      n = ::write(fd, buf_ + r_, Len());
    }
    if (n < 0) {
      return Err(std::strerror(errno));
    }
    if (Len() == 0) {
      Reset();
    }
    return Ok(m);
  }

  ~Buffer() { delete[] buf_; }

protected:
  void AlignToHead() {
    size_t len = Len();
    ::memmove(buf_, buf_ + r_, len);
    w_ = len;
    r_ = 0;
  }

  void ExtendTo(size_t estimated_size) {
    if (estimated_size <= cap_) {
      return;
    }
    cap_ = estimated_size;
    char *newbuf = new char[cap_];
    std::memcpy(newbuf, buf_ + r_, Len());
    delete[] buf_;
    w_ = Len();
    r_ = 0;
    buf_ = newbuf;
  }

private:
  size_t cap_;
  char *buf_;
  size_t r_, w_;
};
}  // namespace kl

#endif
