// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.
#include <thread>

#include "env.h"
#include "filelock.h"
#include "testkit.h"
#include "wait_group.h"

class T {};

TEST(T, Concurrency) {
  kl::WaitGroup sync, sync1;
  auto temp = kl::env::MakeTempFile("filelock");
  ASSERT(temp);
  kl::env::Defer defer([fname = *temp] { kl::env::DeleteFile(fname.c_str()); });
  sync.Add();
  sync1.Add();
  std::thread([ fname = *temp, &sync, &sync1 ] {
    int fd = ::open(fname.c_str(), O_RDONLY);
    ASSERT(fd >= 0);
    kl::env::Defer defer([fd] { ::close(fd); });
    kl::filelock::Lock lock(fd);
    ASSERT(lock.TryLock());
    sync.Done();
    sync1.Wait();
  }).detach();
  int fd = ::open(temp->c_str(), O_RDONLY);
  ASSERT(fd >= 0);
  defer([fd] { ::close(fd); });
  kl::filelock::Lock lock(fd);
  sync.Wait();
  ASSERT(!lock.TryLock(false, true));
  sync1.Done();
}

int main() { return KL_TEST(); }
