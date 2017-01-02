// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#include "logger.h"
#include "testkit.h"

class L {};
TEST(L, DefaultLogger) {
  KL_DEBUG("message from %s", "rofl");
  KL_DEBUG_L(kl::logging::Logger::DefaultLogger(), "message from %s", "wtf");
  auto discard = kl::logging::Logger([](const char *msg) { (void)(msg); });
  KL_DEBUG_L(discard, "message from %s", "imfao");
  kl::logging::Logger::SetDefaultLogger(std::move(discard));
  KL_DEBUG("message from %s", "imfao");
}

int main() {
  kl::test::RunAllTests();
  return 0;
}
