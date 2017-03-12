// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.
#ifndef KL_SLICEH_H_
#define KL_SLICEH_H_
#include <cstring>
#include <string>

namespace kl {

struct Slice {
  const char *data;
  size_t len;
  Slice() : data(""), len(0) {}
  Slice(const Slice &) = default;
  Slice(Slice &&) = default;
  explicit Slice(const std::string &s) : data(s.data()), len(s.size()) {}
  explicit Slice(const char *s) : data(s), len(strlen(s)) {}
  Slice(const char *d, size_t l) : data(d), len(l) {}
  int Compare(const Slice &s) {
    int comp = strncmp(data, s.data, std::min(len, s.len));
    if (comp == 0) {
      return len - s.len;
    }
    return comp;
  }
  std::string ToString() const { return std::string(data, len); }
};

}  // namespace kl
#endif  // KL_SLICEH_H_
