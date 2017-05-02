// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

// Use bits to represent states.

#include <cstdint>
#include <string>

namespace kl {

class States {
public:
  States() = default;
  States(const States &) = default;
  States(States &&) = default;
  bool operator==(const States &rhs);
  bool operator!=(const States &rhs);
  States &Set(uint64_t state);
  bool Contains(uint64_t state) const;
  States &Union(const States &states);
  void Encode(std::string *result);
  bool Decode(const std::string &code);

private:
  std::string states_;
};

}  // namespace kl
