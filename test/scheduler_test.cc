// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include <chrono>
#include <thread>

#include "scheduler.h"
#include "testkit.h"

namespace {
class T {};

TEST(kl::Scheduler, Constructor, 8) {}
TEST(kl::Scheduler, Constructor1, 8, false) {
  auto add = RegisterEpollEvent(1, EPOLLET | EPOLLIN,
                                [](uint32_t events) { (void)events; });
  ASSERT(!add);
  auto del = UnregisterEpollEvent(1);
  ASSERT(!del);
}

TEST(kl::Scheduler, Stop, 8) {
  auto stop_thread = std::thread([this] {
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    Stop("timeout");
  });
  auto status = Go();
  ASSERT(!status);
  stop_thread.join();
}
}  // namespace
