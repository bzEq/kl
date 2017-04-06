// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.
#include <string>

#include "env.h"
#include "logger.h"
#include "subprocess.h"
#include "testkit.h"

class T {};

TEST(T, CallLs) {
  kl::env::ArgvBuilder argv;
  argv.Append("ls");
  auto exit_status = kl::subprocess::Call("ls", argv.Build());
  ASSERT(exit_status);
}

TEST(T, CallGCC) {
  kl::env::ArgvBuilder argv;
  argv.Append("gcc");
  kl::env::EnvpBuilder envp;
  envp.Put("HOME", "/home/error_man");
  auto exit_status = kl::subprocess::Call("gcc", argv.Build(), envp.Build());
  ASSERT(exit_status);
}

int main() { return KL_TEST(); }
