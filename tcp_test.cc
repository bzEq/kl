// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#include "tcp.h"
#include "env.h"
#include "testkit.h"

class T {};

TEST(T, ListenAndSetNonBlocking) {
  auto listen = kl::tcp::Listen("127.0.0.1", 4000);
  ASSERT(listen);
  ASSERT(kl::env::SetNonBlocking(*listen));
  ::close(*listen);
}

int main() { return KL_TEST(); }
