// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.
#ifndef KL_SLICEH_H_
#define KL_SLICEH_H_
#include <string>

namespace kl {

struct Slice {
  const char *data;
  size_t len;
  Slice() : data(nullptr), len(0) {}
  Slice(const Slice &) = default;
  Slice(Slice &&) = default;
  explicit Slice(const std::string &s) : data(s.data()), len(s.size()) {}
  Slice(const char *d, size_t l) : data(d), len(l) {}
};

}  // namespace kl
#endif  // KL_SLICEH_H_
