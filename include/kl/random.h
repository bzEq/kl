// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_RANDOM_H_
#define KL_RANDOM_H_
#include <random>

namespace kl {
namespace random {

template <typename T>
T UniformSample() {
  static std::random_device rd;
  static std::mt19937 g(rd());
  static std::uniform_real_distribution<T> u(0, 1);
  return u(g);
}

inline float UniformSampleFloat() { return UniformSample<float>(); }
inline double UniformSampleDouble() { return UniformSample<double>(); }

inline char RandomPrintableChar() {
  static const char char_begin = 48;  // '0'
  static const char char_end = 122;   // 'z'
  return char_begin + (char_end - char_begin) * UniformSampleFloat();
}

}  // namespace random
}  // namespace kl
#endif
