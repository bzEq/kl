// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_MEMORY_POOL_H_
#define KL_MEMORY_POOL_H_
#include <memory>
#include <vector>

#include "any.h"

namespace kl {

class Allocator {
public:
  template <typename T, typename... Args>
  T *Allocate(Args &&... args) {
    auto p = std::make_unique<T>(std::forward<Args>(args)...);
    T *ret = p.get();
    store_.push_back(std::move(p));
    return ret;
  }

  template <typename T>
  T *AllocateArray(size_t n) {
    auto p = std::make_unique<T[]>(n);
    T *ret = p.get();
    store_.push_back(std::move(p));
    return ret;
  }

private:
  std::vector<Any> store_;
};

}  // namespace kl
#endif  // KL_MEMORY_H_
