// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_HEAP_H_
#define KL_HEAP_H_
#include <cassert>
#include <functional>
#include <vector>

namespace kl {

template <typename Key, typename Comparator = std::less<Key>>
class Heap {
public:
  Heap() = default;

  void Insert(const Key &key) {
    heap_.push_back(key);
    size_t n = heap_.size() - 1;
    while (n > 0) {
      size_t p = (n - 1) >> 1;
      if (compare_(heap_[n], heap_[p])) {
        std::swap(heap_[n], heap_[p]);
      } else {
        break;
      }
      n = p;
    }
  }

  bool Empty() const { return heap_.empty(); }
  size_t Size() const { return heap_.size(); }
  Key &Top() { return heap_.front(); }

  void Pop() {
    if (Empty()) {
      return;
    }
    if (Size() > 1) {
      std::swap(heap_.front(), heap_.back());
    }
    heap_.pop_back();
    size_t n = 0;
    while (true) {
      size_t l = ((n + 1) << 1) - 1;
      size_t r = l + 1;
      size_t c = l;
      if (r < heap_.size() && compare_(heap_[r], heap_[l])) {
        c = r;
      }
      if (c < heap_.size() && compare_(heap_[c], heap_[n])) {
        std::swap(heap_[c], heap_[n]);
        n = c;
      } else {
        break;
      }
    }
  }

private:
  Comparator compare_;
  std::vector<Key> heap_;
};

}  // namespace kl
#endif
