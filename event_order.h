// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_EVENT_ORDER_H_
#define KL_EVENT_ORDER_H_
#include <condition_variable>
#include <mutex>
#include <utility>
#include <vector>

#include "env.h"

namespace kl {
namespace event_order {

class Event {
public:
  friend void HappenedBefore(Event *a, Event *b);
  Event() : done_(false) {}
  Event(const Event &) = delete;
  Event(Event &&) = delete;
  void Happened();

private:
  void Wait();
  std::mutex mu_;
  std::condition_variable cv_;
  bool done_;
  std::vector<Event *> wait_;
};

inline void Event::Wait() {
  std::unique_lock<std::mutex> l(mu_);
  while (!done_) {
    cv_.wait(l);
  }
}

// REQUIRES: called only once
inline void Event::Happened() {
  assert(!done_);
  for (auto &event : wait_) {
    event->Wait();
  }
  std::unique_lock<std::mutex> l(mu_);
  done_ = true;
  l.unlock();
  cv_.notify_all();
}

// TODO(Kai Luo): Cycle detection
inline void HappenedBefore(Event *a, Event *b) { b->wait_.push_back(a); }

}  // namespace event_order
}  // namespace kl

#endif  // KL_EVENT_ORDER_H_
