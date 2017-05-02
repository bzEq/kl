// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "kl/buffer.h"

namespace kl {

size_t Buffer::kMinimumSize = 16;

Buffer::Buffer() : cap_(kMinimumSize), r_(0), w_(0) { buf_ = new char[cap_]; }

Buffer::Buffer(size_t n)
    : cap_(std::max(kMinimumSize, Align(n))), r_(0), w_(0) {
  buf_ = new char[cap_];
}

Buffer::Buffer(const Buffer &b) {
  if (cap_ < b.cap_) {
    delete[] buf_;
    cap_ = b.cap_;
    buf_ = new char[cap_];
  }
  std::memcpy(buf_, b.buf_, b.cap_);
  r_ = b.r_;
  w_ = b.w_;
}

Buffer::Buffer(Buffer &&b) {
  std::swap(cap_, b.cap_);
  std::swap(buf_, b.buf_);
  std::swap(r_, b.r_);
  std::swap(w_, b.w_);
}

Buffer &Buffer::operator=(Buffer &&b) {
  std::swap(cap_, b.cap_);
  std::swap(buf_, b.buf_);
  std::swap(r_, b.r_);
  std::swap(w_, b.w_);
  return *this;
}

std::string Buffer::Peek(size_t n) {
  assert(n > 0);
  n = std::min(n, Len());
  std::string result;
  result.resize(n);
  std::memcpy(&result[0], buf_ + r_, n);
  r_ += n;
  return result;
}

std::string Buffer::PeekAll() {
  size_t n = Len();
  std::string result;
  if (n == 0) {
    return result;
  }
  result = Peek(n);
  Reset();
  return result;
}

Result<size_t> Buffer::ReadFrom(int fd) {
  MakeRoomForReadFrom(1);
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

size_t Buffer::ReadFrom(const char *buf, size_t count) {
  MakeRoomForReadFrom(count);
  std::memcpy(buf_ + w_, buf, count);
  w_ += count;
  return count;
}

Result<size_t> Buffer::WriteTo(int fd) {
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

void Buffer::MakeRoomForReadFrom(size_t count) {
  if (Avail() >= count) {
    return;
  }
  if (Idle() >= count) {
    AlignToHead();
    return;
  }
  ExtendTo(cap_ + count);
}

void Buffer::ExtendTo(size_t estimated_size) {
  if (estimated_size <= cap_) {
    return;
  }
  cap_ = Align(estimated_size);
  char *newbuf = new char[cap_];
  std::memcpy(newbuf, buf_ + r_, Len());
  delete[] buf_;
  w_ = Len();
  r_ = 0;
  buf_ = newbuf;
}

}  // namespace kl
