// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "env.h"
#include "logger.h"
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

TEST(E, TempFile) {
  auto make = kl::env::MakeTempFile("c++");
  ASSERT(make);
  ASSERT(kl::env::FileExists(make->c_str()));
  KL_DEBUG("temp file name: %s", make->c_str());
  ASSERT(kl::env::DeleteFile(make->c_str()));
}

TEST(E, TestDir) {
  std::string dir_name = kl::env::MakeTempName("/tmp/temp");
  ASSERT(kl::env::CreateDir(dir_name.c_str()));
  kl::env::Defer defer([name = dir_name] { kl::env::DeleteDir(name.c_str()); });
  ASSERT(kl::env::IsDir(dir_name.c_str()));
}

int main() { return KL_TEST(); }
