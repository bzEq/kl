// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

// Please use std::any of c++17 if available.

#ifndef KL_UNSAFE_ANY_H_
#define KL_UNSAFE_ANY_H_
#include <memory>
#include <utility>

#include "option.h"

namespace kl {

class Store {
public:
  virtual ~Store() {}
};

template <typename T>
class StoreImpl : public Store {
public:
  StoreImpl(T &&value)
      : pointer_(std::make_unique<T>(std::forward<T>(value))) {}
  const T &Get() { return *pointer_.get(); }

private:
  std::unique_ptr<T> pointer_;
};

class UnsafeAny {
public:
  UnsafeAny() : store_(nullptr) {}

  template <typename T>
  UnsafeAny(T &&value) : store_(new StoreImpl<T>(std::forward<T>(value))) {}

  template <typename TT, typename T = typename std::decay<TT>::type>
  Option<T> Of() {
    if (StoreImpl<T> *store = dynamic_cast<StoreImpl<T> *>(store_)) {
      return store->Get();
    }
    return None();
  }

  ~UnsafeAny() { delete store_; }

private:
  Store *store_;
};

}  // namespace kl
#endif  // KL_UNSAFE_ANY_H_
