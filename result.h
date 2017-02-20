// Copyright (c) 2016 Kai Luo. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_RESULT_H_
#define KL_RESULT_H_
#include <cassert>
#include <memory>

namespace kl {
namespace result {
namespace {
template <typename V>
struct result_value {
  std::unique_ptr<V> v;
};

template <typename E>
struct result_error {
  std::unique_ptr<E> e;
};
}

template <typename V, typename VV = typename std::decay<V>::type>
inline result_value<VV> Ok(V &&v) {
  return result_value<VV>{
      .v = std::make_unique<VV>(std::forward<V>(v)),
  };
}

template <typename E, typename EE = typename std::decay<E>::type>
inline result_error<EE> Err(E &&e) {
  return result_error<EE>{
      .e = std::make_unique<EE>(std::forward<E>(e)),
  };
}

template <typename V, typename E>
class Result {
public:
  Result() : e_(std::make_unique<E>()) {}
  Result(const Result &result) {
    if (result.v_) {
      v_ = std::make_unique<V>(*result.v_);
    }
    if (result.e_) {
      e_ = std::make_unique<E>(*result.e_);
    }
  }
  Result(Result &&) = default;
  Result(result_value<V> &&v) : v_(std::move(v.v)) {}
  Result(result_error<E> &&e) : e_(std::move(e.e)) {}
  Result &operator=(const Result &result) {
    if (result.v_) {
      v_ = std::make_unique<V>(*result.v_);
    }
    if (result.e_) {
      e_ = std::make_unique<E>(*result.e_);
    }
    return *this;
  }
  Result &operator=(Result &&result) {
    v_ = std::move(result.v_);
    e_ = std::move(result.e_);
    return *this;
  }
  Result &operator=(result_value<V> &&v) {
    v_ = std::move(v.v);
    e_ = nullptr;
    return *this;
  }
  Result &operator=(result_error<E> &&e) {
    v_ = nullptr;
    e_ = std::move(e.e);
    return *this;
  }

  explicit operator bool() const { return e_ == nullptr; }
  bool Ok() const { return operator bool(); }

  V &operator*() {
    assert(v_);
    return *v_;
  }

  const V &operator*() const {
    assert(v_);
    return *v_;
  }

  V *operator->() {
    assert(v_);
    return v_.operator->();
  }

  const V *operator->() const {
    assert(v_);
    return v_.operator->();
  }

  V &&Move() {
    assert(v_);
    return std::move(*v_);
  }

  const E &Err() const {
    assert(e_);
    return *e_;
  }

  E &&MoveErr() {
    assert(e_);
    return std::move(*e_);
  }

private:
  std::unique_ptr<V> v_;
  std::unique_ptr<E> e_;
};

template <typename E>
class Result<void, E> {
public:
  Result() : e_(std::make_unique<E>()) {}
  Result(const Result &result) {
    if (result.e_) {
      e_ = std::make_unique<E>(*result.e_);
    }
  }
  Result(Result &&) = default;
  Result(std::nullptr_t) {}
  Result(result_error<E> &&e) : e_(std::move(e.e)) {}
  Result &operator=(const Result &result) {
    if (result.e_) {
      e_ = std::make_unique<E>(*result.e_);
    }
    return *this;
  }

  Result &operator=(Result &&result) {
    e_ = std::move(result.e_);
    return *this;
  }

  Result &operator=(std::nullptr_t) {
    e_ = nullptr;
    return *this;
  }

  Result &operator=(result_error<E> &&e) {
    e_ = std::move(e.e);
    return *this;
  }

  explicit operator bool() const { return e_ == nullptr; }
  bool Ok() const { return operator bool(); }
  const E &Err() const {
    assert(e_);
    return *e_;
  }
  E &&MoveErr() {
    assert(e_);
    return std::move(*e_);
  }

private:
  std::unique_ptr<E> e_;
};

inline std::nullptr_t Ok() { return nullptr; }

}  // namespace result
}  // namespace kl

#endif
