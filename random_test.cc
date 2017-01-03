// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#include "random.h"
#include "testkit.h"

class R {};

TEST(R, UniformSample) {
  float s = kl::random::UniformSampleFloat();
  ASSERT(s >= 0 && s <= 1);
}

int main() { return KL_TEST(); }
