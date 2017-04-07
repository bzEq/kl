// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "logger.h"
#include "testkit.h"

namespace {
class L {};
}

TEST(L, DefaultLogger) {
  KL_DEBUG("message from %s", "rofl");
  KL_DEBUG_L(kl::logging::Logger::DefaultLogger(), "message from %s", "wtf");
  auto discard =
      kl::logging::Logger([](const std::string &msg) { (void)(msg); });
  KL_DEBUG_L(discard, "message from %s", "imfao");
  kl::logging::Logger::SetDefaultLogger(std::move(discard));
  KL_DEBUG("message from %s", "imfao");
}
