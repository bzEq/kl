// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_TIMER_H_
#define KL_TIMER_H_
#include <chrono>

namespace kl {
namespace timer {

// FIXME(Kai Luo): any trick to decrease function overloading?
template <typename T, typename R, typename C, typename... Params,
          typename... Args>
inline R FunctionCost(std::chrono::duration<T> *diff, R (C::*f)(Params...),
                      C *c, Args &&... args) {
  auto start = std::chrono::high_resolution_clock::now();
  auto result = (c->*f)(std::forward<Args>(args)...);
  *diff = std::chrono::high_resolution_clock::now() - start;
  return result;
}

template <typename T, typename C, typename... Params, typename... Args>
inline void FunctionCost(std::chrono::duration<T> *diff,
                         void (C::*f)(Params...), C *c, Args &&... args) {
  auto start = std::chrono::high_resolution_clock::now();
  (c->*f)(std::forward<Args>(args)...);
  *diff = std::chrono::high_resolution_clock::now() - start;
}

template <typename T, typename R, typename... Params, typename... Args>
inline R FunctionCost(std::chrono::duration<T> *diff, R (*f)(Params...),
                      Args &&... args) {
  auto start = std::chrono::high_resolution_clock::now();
  auto result = (*f)(std::forward<Args>(args)...);
  *diff = std::chrono::high_resolution_clock::now() - start;
  return result;
}

template <typename T, typename... Params, typename... Args>
inline void FunctionCost(std::chrono::duration<T> *diff, void (*f)(Params...),
                         Args &&... args) {
  auto start = std::chrono::high_resolution_clock::now();
  (*f)(std::forward<Args>(args)...);
  *diff = std::chrono::high_resolution_clock::now() - start;
}

template <typename T, typename R, typename... Params, typename... Args>
inline R FunctionCost(std::chrono::duration<T> *diff,
                      std::function<R(Params...)> &&func, Args &&... args) {
  auto start = std::chrono::high_resolution_clock::now();
  auto result = func(std::forward<Args>(args)...);
  *diff = std::chrono::high_resolution_clock::now() - start;
  return result;
}

template <typename T, typename... Params, typename... Args>
inline void FunctionCost(std::chrono::duration<T> *diff,
                         std::function<void(Params...)> &&func,
                         Args &&... args) {
  auto start = std::chrono::high_resolution_clock::now();
  func(std::forward<Args>(args)...);
  *diff = std::chrono::high_resolution_clock::now() - start;
}

}  // namespace timer
}  // namespace kl
#endif
