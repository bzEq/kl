// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_STATIC_VALUE_H_
#define KL_STATIC_VALUE_H_

namespace kl {

// REQUIRES: Has default constructor
template <typename T> class StaticValue {
public:
  T &operator*() { return *ref(); }

private:
  T *ref() {
    static T instance;
    return &instance;
  }
};

} // namespace kl
#endif // KL_STATIC_VALUE_H_
