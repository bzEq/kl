// Copyright (c) 2016 Kai Luo. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_BUFFER_H_
#define KL_BUFFER_H_
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <iostream>
#include <utility>
#include <vector>

#include "error.h"

namespace kl {

class Buffer {
public:
  Buffer() : cap_(16), r_(0), w_(0) { buf_ = new char[cap_]; }

  explicit Buffer(int n) : cap_(std::max(16, n)), r_(0), w_(0) {
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
  int Len() const { return w_ - r_; }
  int Cap() const { return cap_; }
  bool Empty() const { return r_ == w_; }
  int Avail() const { return cap_ - w_; }

  std::vector<char> Peek(int n) {
    n = std::min(n, Len());
    std::vector<char> res(n);
    std::memcpy(res.data(), buf_ + r_, n);
    r_ += n;
    return res;
  }

  std::vector<char> PeekAll() {
    int n = Len();
    std::vector<char> res(n);
    std::memcpy(res.data(), buf_ + r_, n);
    Reset();
    return res;
  }

  Result<int> ReadFrom(int fd) {
    assert(Avail() > 0);
    int m = 0;
    int n = ::read(fd, buf_ + w_, Avail());
    while (n > 0) {
      w_ += n;
      m += n;
      if (Avail() == 0) {
        ExtendTo(2 * cap_);
      }
      n = ::read(fd, buf_ + w_, Avail());
    }
    if (n < 0) {
      return Err(std::strerror(errno));
    }
    return Ok(m);
  }

  int ReadFrom(const char *buf, int count) {
    if (Avail() < count) {
      ExtendTo(w_ + count);
    }
    int n = std::min(count, Avail());
    std::memcpy(buf_ + w_, buf, n);
    w_ += n;
    return n;
  }

  int ReadFrom(const std::vector<char> &buf) {
    return ReadFrom(buf.data(), buf.size());
  }

  Result<int> WriteTo(int fd) {
    if (Len() == 0) {
      return Ok(0);
    }
    int m = 0;
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
  void ExtendTo(int n) {
    if (n <= cap_) {
      return;
    }
    cap_ = n;
    char *newbuf = new char[cap_];
    std::memcpy(newbuf, buf_ + r_, Len());
    delete[] buf_;
    w_ = Len();
    r_ = 0;
    buf_ = newbuf;
  }

private:
  int cap_;
  char *buf_;
  int r_, w_;
};
}  // namespace kl

#endif
