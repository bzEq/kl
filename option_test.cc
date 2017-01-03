// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#include <iostream>
#include <memory>
#include <string>

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

struct Imfao {
  std::string s;
};

kl::Option<std::string> Bar() {
  auto f = std::make_unique<Imfao>();
  return f->s;
}

TEST(O, PassByValue) {
  auto f = Bar();
  ASSERT(f);
}

int main() { return KL_TEST(); }
