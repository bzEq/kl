// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include <cassert>

#include "slice.h"

namespace kl {
namespace coding {

template <typename T>
inline T GetBigEndianInteger(const Slice &s) {
  assert(sizeof(T) <= s.len);
  T result = 0;
  for (size_t i = 0; i < sizeof(T); ++i) {
    result = (result << 8) | *reinterpret_cast<const uint8_t *>(s.data + i);
  }
  return result;
}

}  // namespapce coding
}  // namespace kl
