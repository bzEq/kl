// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.
#ifndef KL_FILELOCK_H_
#define KL_FILELOCK_H_
#include <sys/file.h>

#include <cerrno>
#include <cstring>
#include <stdexcept>

namespace kl {
namespace filelock {

class Lock {
public:
  explicit Lock(int fd) : locked_(false), fd_(fd) {}
  bool TryLock() {
    if (locked_) {
      return true;
    }
    int err = flock(fd_, LOCK_EX);
    if (err < 0) {
      return false;
    }
    locked_ = true;
    return true;
  }

  bool TryLock(bool shared, bool nonblocking) {
    if (locked_) {
      return true;
    }
    int flags = LOCK_EX;
    if (shared) {
      flags = LOCK_SH;
    }
    if (nonblocking) {
      flags |= LOCK_NB;
    }
    int err = flock(fd_, flags);
    if (err < 0) {
      return false;
    }
    locked_ = true;
    return true;
  }

  void Release() {
    if (locked_) {
      int err = flock(fd_, LOCK_UN);
      if (err < 0) {
        throw std::runtime_error(std::strerror(errno));
      }
      locked_ = false;
    }
  }

  ~Lock() { Release(); }

private:
  bool locked_;
  int fd_;
};
}  // namespace filelock
}  // namespace kl
#endif  // KL_FILELOCK_H_
