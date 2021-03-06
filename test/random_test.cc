// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include <algorithm>

#include "kl/random.h"
#include "kl/testkit.h"

namespace {
class R {};

TEST(R, UniformSample) {
  static const float kEPS = 1e-2;
  static const int kNumOfSamples = 1 << 16;
  float sum = 0;
  for (int i = 0; i < kNumOfSamples; ++i) {
    float s = kl::random::UniformSampleFloat();
    ASSERT(s >= 0 && s <= 1);
    sum += s;
  }
  sum /= kNumOfSamples;
  ASSERT(std::abs(sum - 0.5) <= kEPS);
}
}  // namespace
