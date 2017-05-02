// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include <algorithm>

#include "kl/states.h"

namespace kl {

bool States::operator==(const States &rhs) {
  // Since States hasn't got a clear operation yet,
  // just simply use std::string's operator==.
  return states_ == rhs.states_;
}

bool States::operator!=(const States &rhs) { return states_ != rhs.states_; }

States &States::Set(uint64_t state) {
  if (states_.size() << 3 <= state) {
    states_.resize((state >> 3) + 1, 0);
  }
  size_t index = state >> 3;
  size_t bit = state & 0x7;
  states_[index] |= (1 << bit);
  return *this;
}

bool States::Contains(uint64_t state) const {
  size_t index = state >> 3;
  if (index > states_.size()) {
    return false;
  }
  size_t bit = state & 0x7;
  return states_[index] & (1 << bit);
}

States &States::Union(const States &rhs) {
  for (size_t i = 0; i < std::min(states_.size(), rhs.states_.size()); ++i) {
    states_[i] |= rhs.states_[i];
  }
  return *this;
}

void States::Encode(std::string *result) { *result = states_; }

bool States::Decode(const std::string &code) {
  states_ = code;
  return true;
}

}  // namespace kl
