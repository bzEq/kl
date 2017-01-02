// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#ifndef KL_OPTION_H_
#define KL_OPTION_H_
#include <cassert>
#include <memory>

namespace kl {

template <typename T>
class Option {
public:
  Option() = default;
  Option(const Option &) = delete;
  Option(Option &&) = default;
  Option &operator=(Option &&o) { v_ = std::move(o.v_); }
  Option(T &&v) : v_(std::make_unique<T>(std::forward<T>(v))) {}
  Option(std::nullptr_t) {}
  explicit operator bool() const { return v_ != nullptr; }

  T &operator*() {
    assert(v_);
    return *v_;
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
