// Copyright (c) 2016 Kai Luo. All rights reserved.

#ifndef KL_BITSET_H_
#define KL_BITSET_H_
#include <algorithm>
#include <mutex>
#include <thread>

namespace kl {

namespace {

inline size_t CountZeroBits(uint8_t x) {
  if (x == 0) {
    return 8;
  }
  size_t n = 1;
  if ((x & 0x0f) == 0) {
    n += 4;
    x >>= 4;
  }
  if ((x & 0x03) == 0) {
    n += 2;
    x >>= 2;
  }
  return n - (x & 1);
}
inline int AlignTo8(int n) { return (n + 7) & (~7); }
}

// thread safe bitset
class BitSet {
public:
  explicit BitSet(size_t size)
      : set_(nullptr), size_(std::max(1 << 3, AlignTo8(size))) {
    set_ = new uint8_t[size_ >> 3];
  }
  BitSet(const BitSet &) = delete;
  BitSet(BitSet &&) = delete;

  void Set(int index) {
    std::unique_lock<std::mutex> _(mu_);
    if (index >= 0 && index < size_) {
      int i = index >> 3;
      int k = index & 7;
      set_[i] |= (1 << k);
    }
  }

  void Clear(int index) {
    std::unique_lock<std::mutex> _(mu_);
    if (index >= 0 && index < size_) {
      int i = index >> 3;
      int k = index & 7;
      set_[i] &= ~(1 << k);
    }
  }

  int Test(int index) {
    std::unique_lock<std::mutex> _(mu_);
    if (index >= 0 && index < size_) {
      int i = index >> 3;
      int k = index & 7;
      return set_[i] & (1 << k);
    }
    return 0;
  }

  int SetFirstZeroBit() {
    std::unique_lock<std::mutex> _(mu_);
    for (int i = 0; i < (size_ >> 3); ++i) {
      int k = CountZeroBits(~set_[i]);
      if (k >= 0 && k < 8) {
        int index = (i << 3) + k;
        set_[i] |= (1 << k);
        return index;
      }
    }
    return -1;
  }

  size_t Size() const { return size_; }

  ~BitSet() { delete[] set_; }

private:
  uint8_t *set_;
  std::mutex mu_;
  size_t size_;
};

}  // namespace raft
#endif
