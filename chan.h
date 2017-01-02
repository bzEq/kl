// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef KL_CHAN_H_
#define KL_CHAN_H_
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

#include "option.h"

namespace kl {

// a blocking channel impl
template <typename T>
class Chan {
public:
  Chan() : closed_(false) {}
  Chan(const Chan &c) = delete;
  Chan(Chan &&c) = delete;

  void Close() { closed_.store(true); }
  bool Closed() { return closed_; }

  // TODO(Kai Luo): version of dealing with time out is required.
  friend bool operator<<(Chan &c, T &&v) {
    std::unique_lock<std::mutex> l(c.mu_);
    if (c.closed_) {
      return false;
    }
    c.q_.push(std::forward<T>(v));
    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again.
    l.unlock();
    c.cv_.notify_one();
    return true;
  }

  bool Push(T &&v) { return *this << std::forward<T>(v); }

  Option<T> Pop() {
    std::unique_lock<std::mutex> l(mu_);
    while (!closed_ && q_.empty()) {
      cv_.wait(l);
    }
    if (q_.empty()) {
      return None();
    }
    auto f(std::move(q_.front()));
    q_.pop();
    l.unlock();
    return std::move(f);
  }

private:
  std::mutex mu_;
  std::condition_variable cv_;
  std::queue<T> q_;
  std::atomic<bool> closed_;
};

}  // namespace kl
#endif
