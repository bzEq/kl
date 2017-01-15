// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "env.h"
#include "testkit.h"

class E {};

TEST(E, CreateAndDeleteDir) {
  static const char *dir = "/tmp/env_test";
  ASSERT(kl::env::CreateDir(dir));
  ASSERT(kl::env::FileExists(dir));
  ASSERT(kl::env::DeleteDir(dir));
}

TEST(E, FillFileContent) {
  static const char *file = "/tmp/env_test.txt";
  std::string data("// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All "
                   "rights reserved.");
  auto fill = kl::env::FillFileWith(file, data);
  ASSERT(fill);
  auto read = kl::env::ReadFile(file);
  ASSERT(read);
  ASSERT(*read == data);
  ASSERT(kl::env::DeleteFile(file));
}

TEST(E, FileExists) {
  static const char *dir = "/usr/";
  ASSERT(kl::env::FileExists(dir));
}

int main() { return KL_TEST(); }
