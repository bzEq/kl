// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_OPTION_H_
#define KL_OPTION_H_
#include <cassert>
#include <cstddef>
#include <memory>

namespace kl {

template <typename T>
class Option {
public:
  Option() = default;
  Option(const Option &) = delete;
  Option(Option &&) = default;
  Option &operator=(Option &&o) {
    v_ = std::move(o.v_);
    return *this;
  }
  Option(const T &v) : v_(std::make_unique<T>(v)) {}
  Option(T &&v) : v_(std::make_unique<T>(std::forward<T>(v))) {}
  Option(std::nullptr_t) {}
  explicit operator bool() const { return v_ != nullptr; }

  T &operator*() {
    assert(v_);
    return *v_;
  }

  T *operator->() {
    assert(v_);
    return v_.operator->();
  }

  const T *operator->() const {
    assert(v_);
    return v_.operator->();
  }

  const T &operator*() const {
    assert(v_);
    return *v_;
  }

private:
  std::unique_ptr<T> v_;
};

template <typename TT, typename T = typename std::decay<TT>::type>
inline Option<T> Some(TT &&v) {
  return Option<T>(std::forward<TT>(v));
}

inline std::nullptr_t None() { return nullptr; }

}  // namespace kl
#endif
