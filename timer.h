// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_TIMER_H_
#define KL_TIMER_H_
#include <chrono>

namespace kl {
namespace timer {
// FIXME(Kai Luo): any trick to decrease function overloading?

template <typename T, typename F>
inline void FunctionCost(std::chrono::duration<T> *cost, F &&func) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  *cost = std::chrono::high_resolution_clock::now() - start;
}

template <typename T, typename F, typename... Args>
inline void FunctionCost(std::chrono::duration<T> *cost, F &&func,
                         Args &&... args) {
  auto start = std::chrono::high_resolution_clock::now();
  func(std::forward<Args>(args)...);
  *cost = std::chrono::high_resolution_clock::now() - start;
}

template <typename T, typename R, typename F>
inline void FunctionCost(std::chrono::duration<T> *cost, R *result, F &&func) {
  auto start = std::chrono::high_resolution_clock::now();
  *result = func();
  *cost = std::chrono::high_resolution_clock::now() - start;
}

template <typename T, typename R, typename F, typename... Args>
inline void FunctionCost(std::chrono::duration<T> *cost, R *result, F &&func,
                         Args &&... args) {
  auto start = std::chrono::high_resolution_clock::now();
  *result = func(std::forward<Args>(args)...);
  *cost = std::chrono::high_resolution_clock::now() - start;
}

}  // namespace timer
}  // namespace kl
#endif
