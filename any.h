// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

// Please use std::any of c++17 if available.

#ifndef KL_ANY_H_
#define KL_ANY_H_
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
  StoreImpl(T &&value) : store_(std::make_unique<T>(std::forward<T>(value))) {}
  const T &Get() { return *store_; }

private:
  std::unique_ptr<T> store_;
};

// it seems safe.
class Any {
public:
  Any() : store_(nullptr) {}

  template <typename T>
  Any(T &&value) : store_(new StoreImpl<T>(std::forward<T>(value))) {}

  template <typename T>
  Option<T> Of() {
    if (StoreImpl<T> *store = dynamic_cast<StoreImpl<T> *>(store_)) {
      return store->Get();
    }
    return None();
  }

  ~Any() { delete store_; }

private:
  Store *store_;
};

}  // namespace kl
#endif  // KL_ANY_H_
