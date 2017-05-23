// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

// Please use std::any of c++17 if available.

#ifndef KL_ANY_H_
#define KL_ANY_H_
#include <memory>
#include <utility>

#include "kl/option.h"

namespace kl {

class Any {
public:
  Any() : store_(nullptr) {}
  Any(const Any &) = delete;
  Any(Any &&any) : store_(nullptr) { std::swap(store_, any.store_); }

  Any &operator=(const Any &) = delete;
  Any &operator=(Any &&rhs) {
    std::swap(store_, rhs.store_);
    return *this;
  }

  template <typename T, typename V = typename std::decay<T>::type>
  Any(T &&value) : store_(new StoreImpl<V>(std::forward<T>(value))) {}

  template <typename T, typename V = typename std::decay<T>::type>
  Any &operator=(T &&value) {
    delete store_;
    store_ = new StoreImpl<V>(std::forward<T>(value));
    return *this;
  }

  template <typename T>
  T *Of() {
    StoreImpl<T> *store = dynamic_cast<StoreImpl<T> *>(store_);
    if (!store) {
      return nullptr;
    }
    return store->Get();
  }

  template <typename T>
  bool IsOf() const {
    return dynamic_cast<const StoreImpl<T> *>(store_) != nullptr;
  }

  ~Any() { delete store_; }

private:
  class Store {
  public:
    virtual ~Store() {}
  };

  template <typename T>
  class StoreImpl : public Store {
  public:
    template <typename V>
    StoreImpl(V &&value) : store_(new T(std::forward<V>(value))) {}

    T *Get() { return store_; }

    ~StoreImpl() { delete store_; }

  private:
    T *store_;
  };

  Store *store_;
};

}  // namespace kl
#endif  // KL_ANY_H_
