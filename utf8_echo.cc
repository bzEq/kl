// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

// Echo utf8 string that passed via argv.

#include <cassert>
#include <cstdio>
#include <cstdlib>

#include <iostream>

#include "slice.h"
#include "utf8.h"

int main(int argc, char *argv[]) {
  assert(argc > 0);
  if (argc != 2) {
    printf("Usage: %s <string>\n", argv[0]);
    exit(1);
  }
  kl::utf8::Iterator iter(argv[1]);
  size_t len = 0;
  kl::Slice s;
  iter.Next(&s);
  while (s.len) {
    for (size_t k = 0; k < s.len; ++k) {
      putchar(s.data[k]);
    }
    ++len;
    iter.Next(&s);
  }
  puts("");
  return 0;
}
