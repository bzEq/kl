// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

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
TEST(E, Constructor) {
  std::string str("wtf");
  kl::Result<std::string> ok(
      kl::MakeResult<std::string>(str.begin(), str.end()));
  ASSERT(ok);
  kl::Result<std::string> ok1(kl::Ok(std::move(str)));
  ASSERT(ok1);
  ASSERT(str.empty());
}

int main() { return KL_TEST(); }
