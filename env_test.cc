// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "env.h"
#include "logger.h"
#include "testkit.h"

class E {};

TEST(E, SafeInitialize) {
  kl::env::SafeInitializer<std::string> my_name;
  // only one template function is generated.
  std::string name("wtf");
  my_name.InitAndGet(name);
  ASSERT(*my_name.Get() == "wtf");
  name = "rofl";
  my_name.InitAndGet(name);
  ASSERT(*my_name.Get() == "wtf");
}

TEST(E, SafeInitialize1) {
  kl::env::SafeInitializer<std::string> my_name;
  // two template functions are generated,
  // since char[4] differs from char[5].
  my_name.InitAndGet("wtf");
  ASSERT(*my_name.Get() == "wtf");
  my_name.InitAndGet("rofl");
  ASSERT(*my_name.Get() == "rofl");
}

TEST(E, EnvpBuilder) {
  kl::env::EnvpBuilder builder;
  builder.Put("HOME", "/home/kyne");
  builder.Put("PWD", "/tmp");
  auto result = builder.Build();
  ASSERT(result.size() == 2);
  ASSERT(std::string(result[0]) == "HOME=/home/kyne");
  ASSERT(std::string(result[1]) == "PWD=/tmp");
}

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

TEST(E, Children) {
  auto children = kl::env::GetChildren("./");
  ASSERT(children);
  std::string result;
  for (const auto &c : *children) {
    result.append(c.data(), c.size());
    result.append("\n");
  }
  KL_DEBUG(result.c_str());
}

TEST(E, WritePidToFile) {
  std::string fname = kl::env::MakeTempName("/tmp/env_test");
  kl::env::Defer defer([fname] { kl::env::DeleteFile(fname.c_str()); });
  auto status = kl::env::WritePidToFile(fname.c_str());
  ASSERT(status);
  auto data = kl::env::ReadFile(fname.c_str());
  ASSERT(data);
  auto pid = std::strtoll(data->c_str(), nullptr, 10);
  ASSERT(pid == getpid());
}

int main() { return KL_TEST(); }
