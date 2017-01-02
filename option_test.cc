// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#include <iostream>

#include "option.h"
#include "testkit.h"

class O {};

kl::Option<int> Foo() { return kl::Some(13); }
kl::Option<int> Spam() { return kl::None(); }

TEST(O, Option) {
  auto foo = Foo();
  ASSERT(foo);
  std::cout << *foo << std::endl;
  auto spam = Spam();
  ASSERT(!spam);
}

int main() {
  kl::test::RunAllTests();
  return 0;
}