// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include <unistd.h>

#include "kl/logger.h"
#include "kl/testkit.h"
#include "kl/env.h"

#include <iostream>

namespace {
class L {};

TEST(L, DefaultLogger) {
  KL_DEBUG("message from %s", "rofl");
  KL_DEBUG_L(kl::logging::Logger::DefaultLogger(), "message from %s", "wtf");
  auto discard =
      kl::logging::Logger([](const std::string &msg) { (void)(msg); });
  KL_DEBUG_L(discard, "message from %s", "imfao");
  kl::logging::Logger::SetDefaultLogger(std::move(discard));
  KL_DEBUG("message from %s", "imfao");
}

TEST(L, SetOutputFile) {
  // auto filename = kl::env::MakeTempFile("logger_test");
  // ASSERT(filename);
  // std::cerr << filename->c_str() << "\n";
  // ASSERT(kl::env::FileExists(filename->c_str()));
  int fd = ::open("/tmp/logger_test", O_CREAT | O_WRONLY, 0644);
  ASSERT(fd >= 0);
  kl::env::Defer defer([fd] { ::close(fd); });
  kl::logging::Logger::SetDefaultLogger(kl::logging::Logger(
      kl::logging::kError, [fd](const std::string &message) {
        int nwrite = ::write(fd, message.data(), message.size());
        (void)nwrite;
      }));
  KL_ERROR("wtf???");
}

} // namespace
