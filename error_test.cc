// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include "error.h"
#include "testkit.h"

class E {};

static kl::Status Ok() { return kl::Ok(); }
static kl::Status Error() { return kl::Err("error"); }

TEST(E, MoveErrString) {
  kl::Result<int> res(kl::Err("wtf"));
  std::string moved(res.MoveErr().MoveString());
  ASSERT(moved == "wtf");
  ASSERT(res.Err().ToString() == "");
}

TEST(E, StatusOk) { ASSERT(Ok()); }
TEST(E, StatusError) { ASSERT(!Error()); }

int main() { return KL_TEST(); }
