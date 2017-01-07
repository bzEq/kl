// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#ifndef KL_TIMER_H_
#define KL_TIMER_H_
#include <chrono>

namespace kl {
namespace timer {

template <typename T, typename R, typename C, typename... Params,
          typename... Args>
inline R FunctionCost(std::chrono::duration<T> *diff, R (C::*f)(Params...),
                      C *c, Args &&... args) {
  auto start = std::chrono::high_resolution_clock::now();
  auto result = (c->*f)(std::forward<Args>(args)...);
  *diff = std::chrono::high_resolution_clock::now() - start;
  return result;
}

template <typename T, typename R, typename... Params, typename... Args>
inline R FunctionCost(std::chrono::duration<T> *diff, R (*f)(Params...),
                      Args &&... args) {
  auto start = std::chrono::high_resolution_clock::now();
  auto result = (*f)(std::forward<Args>(args)...);
  *diff = std::chrono::high_resolution_clock::now() - start;
  return result;
}

}  // namespace timer
}  // namespace kl
#endif
