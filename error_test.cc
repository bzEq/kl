// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include "error.h"
#include "testkit.h"

class E {};

TEST(E, MoveErrString) {
  kl::Result<int> res(kl::Err("wtf"));
  std::string moved(res.MoveErr().MoveString());
  ASSERT(moved == "wtf");
  ASSERT(res.Err().ToString() == "");
}

int main() { return KL_TEST(); }
