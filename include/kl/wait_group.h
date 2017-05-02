// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_WAIT_GROUP_H_
#define KL_WAIT_GROUP_H_
#include <condition_variable>
#include <mutex>
#include <utility>

namespace kl {
class WaitGroup {
public:
  WaitGroup() : counter_(0) {}
  WaitGroup(const WaitGroup &) = delete;
  WaitGroup(WaitGroup &&) = delete;

  void Add() {
    std::unique_lock<std::mutex> _(mu_);
    ++counter_;
  }

  void Done() {
    std::unique_lock<std::mutex> l(mu_);
    --counter_;
    if (counter_ == 0) {
      l.unlock();
      cv_.notify_all();
    }
  }

  void Wait() {
    std::unique_lock<std::mutex> l(mu_);
    while (counter_ > 0) {
      cv_.wait(l);
    }
  }

  size_t Counter() {
    std::unique_lock<std::mutex> l(mu_);
    size_t ret = counter_;
    return ret;
  }

private:
  size_t counter_;
  std::mutex mu_;
  std::condition_variable cv_;
};
}  // namespace kl
#endif
