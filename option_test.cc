// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#include <iostream>

#include "option.h"
#include "testkit.h"

class O {};

kl::Option<int> Foo() { return kl::Some(13); }
kl::Option<int> Spam() { return kl::None(); }

TEST(O, foo) {
  auto foo = Foo();
  ASSERT(foo);
  ASSERT(*foo == 13);
}

TEST(O, spam) {
  auto spam = Spam();
  ASSERT(!spam);
}

int main() { return KL_TEST(); }
