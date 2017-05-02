// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_SUBPROCESS_H_
#define KL_SUBPROCESS_H_
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>
#include <string>
#include <vector>

#include "kl/error.h"

namespace kl {
namespace subprocess {

Result<pid_t> Fork() {
  pid_t child = fork();
  if (child < 0) {
    return Err(std::strerror(errno));
  }
  return Ok(child);
}

Result<int> Wait() {
  int err, status;
  err = wait(&status);
  if (err < 0) {
    return Err(std::strerror(errno));
  }
  return Ok(status);
}

Result<int> Call(const std::string &executable, const std::vector<char *> &argv,
                 const std::vector<char *> &envp) {
  auto child = Fork();
  if (!child) {
    return Err(child.MoveErr());
  }
  if (*child == 0) {
    int err = execvpe(executable.data(), argv.data(), envp.data());
    if (err < 0) {
      exit(err);
    }
    // never reach here
    assert(false);
  }
  return Wait();
}

Result<int> Call(const std::string &executable,
                 const std::vector<char *> &argv) {
  auto child = Fork();
  if (!child) {
    return Err(child.MoveErr());
  }
  if (*child == 0) {
    int err = execvp(executable.data(), argv.data());
    if (err < 0) {
      exit(err);
    }
    // never reach here
    assert(false);
  }
  return Wait();
}

}  // namespace subprocess
}  // namespace kl
#endif  // KL_SUBPROCESS_H_
