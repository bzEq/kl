// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_RWLOCK_H_
#define KL_RWLOCK_H_
#include <pthread.h>

namespace kl {
namespace rwlock {

class Lock {
public:
  Lock() { pthread_rwlock_init(&lock_, nullptr); }
  void RLock() { pthread_rwlock_rdlock(&lock_); }
  void WLock() { pthread_rwlock_wrlock(&lock_); }
  void RUnlock() { pthread_rwlock_unlock(&lock_); }
  void WUnlock() { pthread_rwlock_unlock(&lock_); }
  ~Lock() { pthread_rwlock_destroy(&lock_); }

private:
  pthread_rwlock_t lock_;
};

class RDGuard {
public:
  explicit RDGuard(Lock *lock) : lock_(lock) {
    lock_->RLock();
    locked_ = true;
  }
  void Unlock() {
    if (locked_) {
      locked_ = false;
      lock_->RUnlock();
    }
  }
  ~RDGuard() { Unlock(); }

private:
  bool locked_;
  Lock *lock_;
};

class WRGuard {
public:
  explicit WRGuard(Lock *lock) : lock_(lock) {
    locked_ = true;
    lock_->WLock();
  }
  void Unlock() {
    if (locked_) {
      lock_->WUnlock();
      locked_ = false;
    }
  }
  ~WRGuard() { Unlock(); }

private:
  bool locked_;
  Lock *lock_;
};

}  // namespace rwlock
}  // namespace kl
#endif
