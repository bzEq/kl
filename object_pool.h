// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.
#ifndef KL_OBJECT_POOL_H_
#define KL_OBJECT_POOL_H_
#include <cassert>
#include <vector>

namespace kl {
template <typename T>
class ObjectPool {
public:
  ObjectPool() = default;

  template <typename... Args>
  T *New(Args &&... args) {
    auto p = new T(std::forward<Args>(args)...);
    pool_.push_back(p);
    return p;
  }

  template <typename... Args>
  size_t NewAt(Args &&... args) {
    New(std::forward<Args>(args)...);
    return pool_.size() - 1;
  }

  T *At(size_t index) {
    if (index < pool_.size()) {
      return pool_[index];
    }
    return nullptr;
  }

  void Delete() {
    for (auto &p : pool_) {
      delete p;
    }
    pool_.clear();
    assert(pool_.empty());
  }

  ~ObjectPool() { Delete(); }

private:
  std::vector<T *> pool_;
};

}  // namespace kl
#endif  // KL_OBJECT_POOL_H_
