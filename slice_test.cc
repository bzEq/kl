// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "slice.h"
#include "testkit.h"

class T {};

TEST(T, Compare) {
  const char *str = "wtf";
  kl::Slice s(str);
  kl::Slice t(str);
  ASSERT(s.Compare(t) == 0);
}

TEST(T, Compare1) {
  const char *str = "wtf";
  const char *str1 = "wtf!";
  kl::Slice s(str);
  kl::Slice t(str1);
  ASSERT(s.Compare(t) < 0);
}

TEST(T, Compare2) {
  const char *str = "wwtf";
  const char *str1 = "wtf!";
  kl::Slice s(str);
  kl::Slice t(str1);
  ASSERT(s.Compare(t) > 0);
}

int main() { return KL_TEST(); }
