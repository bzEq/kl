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
private:
  static void DeleteArray(T *p) { delete[] p; }
  static void DeleteNormal(T *p) { delete p; }

public:
  ObjectPool() = default;

  template <typename... Args>
  T *New(Args &&... args) {
    auto p = new T(std::forward<Args>(args)...);
    pool_.push_back(p);
    deleters_.push_back(&DeleteNormal);
    return p;
  }

  // REQUIRES: T has default constructor.
  T *NewArray(size_t n) {
    auto p = new T[n]();
    pool_.push_back(p);
    deleters_.push_back(&DeleteArray);
    return p;
  }

  template <typename... Args>
  size_t NewAt(Args &&... args) {
    New(std::forward<Args>(args)...);
    return pool_.size() - 1;
  }

  size_t NewArrayAt(size_t n) {
    NewArrayAt(n);
    return pool_.size() - 1;
  }

  T *At(size_t index) {
    if (index < pool_.size()) {
      return pool_[index];
    }
    return nullptr;
  }

  size_t Size() { return pool_.size(); }

  bool Empty() { return pool_.empty(); }

  void Delete() {
    assert(pool_.size() == deleters_.size());
    for (size_t i = 0; i < pool_.size(); ++i) {
      deleters_[i](pool_[i]);
    }
    pool_.clear();
    deleters_.clear();
    assert(pool_.empty());
    assert(deleters_.empty());
  }

  ~ObjectPool() { Delete(); }

private:
  typedef void (*DeleterType)(T *);
  std::vector<T *> pool_;
  std::vector<DeleterType> deleters_;
};

}  // namespace kl
#endif  // KL_OBJECT_POOL_H_
