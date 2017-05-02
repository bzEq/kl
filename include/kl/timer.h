// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_TIMER_H_
#define KL_TIMER_H_
#include <chrono>

#include "kl/env.h"

namespace kl {
namespace timer {

template <typename T, typename F, typename... Args>
inline auto FunctionCost(std::chrono::duration<T> *cost, F &&func,
                         Args &&... args) {
  auto start = std::chrono::high_resolution_clock::now();
  kl::env::Defer defer([ start = std::move(start), cost ] {
    *cost = std::chrono::high_resolution_clock::now() - start;
  });
  return func(std::forward<Args>(args)...);
}

}  // namespace timer
}  // namespace kl
#endif
