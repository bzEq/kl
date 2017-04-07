// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include <iostream>
#include <memory>
#include <string>

#include "option.h"
#include "testkit.h"

namespace {
class O {};
kl::Option<int> Foo() { return kl::Some(13); }
kl::Option<int> Spam() { return kl::None(); }
}

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

class Foobar {
public:
  int Value() { return 7; }
  std::string String() const { return "Foobar"; }
};

TEST(O, MethodAccess) {
  auto f = kl::Some(Foobar());
  ASSERT(f->Value() == 7);
  ASSERT(f->String() == "Foobar");
}
