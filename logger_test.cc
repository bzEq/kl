// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#include "logger.h"
#include "logger.h"
#include "testkit.h"

class L {};
TEST(L, DefaultLogger) {
  KL_DEBUG("message from %s", "rofl");
  KL_DEBUG_L(kl::logging::Logger::DefaultLogger(), "message from %s", "wtf");
}

int main() {
  kl::test::RunAllTests();
  return 0;
}
