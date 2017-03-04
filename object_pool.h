// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.
#ifndef KL_OBJECT_POOL_H_
#define KL_OBJECT_POOL_H_
#include <vector>

namespace kl {
template <typename T>
class ObjectPool {
public:
  ObjectPool() = default;

  template <typename... Args>
  T *Allocate(Args &&... args) {
    auto p = new T(std::forward<Args>(args)...);
    pool_.push_back(p);
    return p;
  }

  ~ObjectPool() {
    for (auto p : pool_) {
      delete p;
    }
  }

private:
  std::vector<T *> pool_;
};

}  // namespace kl
#endif  // KL_OBJECT_POOL_H_
