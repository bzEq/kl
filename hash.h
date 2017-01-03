// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#ifndef KL_HASH_H_
#define KL_HASH_H_
#include <cstdint>

namespace kl {
namespace hash {

inline uint16_t Fletcher16(const char *data, size_t len) {
  uint16_t s = 0, t = 0;
  for (size_t i = 0; i < len; ++i) {
    s = (s + static_cast<uint8_t>(data[i])) % 255;
    t = (s + t) % 255;
  }
  return t << 8 | s;
}

}  // namespace hash
}  // namespace kl
#endif
