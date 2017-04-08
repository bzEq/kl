// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.
#include <thread>

#include "env.h"
#include "event_order.h"
#include "filelock.h"
#include "testkit.h"

namespace {
class T {};

TEST(T, Concurrency) {
  auto temp = kl::env::MakeTempFile("filelock");
  ASSERT(temp);
  kl::env::Defer defer([fname = *temp] { kl::env::DeleteFile(fname.c_str()); });
  kl::event_order::Event locked0, trylock1, before_unlock0, trylock1_done;
  kl::event_order::HappenedBefore(&locked0, &trylock1);
  kl::event_order::HappenedBefore(&trylock1_done, &before_unlock0);
  std::thread([ fname = *temp, &locked0, &before_unlock0 ] {
    int fd = ::open(fname.c_str(), O_RDONLY);
    ASSERT(fd >= 0);
    kl::env::Defer defer([fd] { ::close(fd); });
    kl::filelock::Lock lock(fd);
    ASSERT(lock.TryLock());
    locked0.Happened();
    before_unlock0.Happened();
  }).detach();
  int fd = ::open(temp->c_str(), O_RDONLY);
  ASSERT(fd >= 0);
  defer([fd] { ::close(fd); });
  kl::filelock::Lock lock(fd);
  trylock1.Happened();
  ASSERT(!lock.TryLock(false, true));
  trylock1_done.Happened();
}
}  // namespace
