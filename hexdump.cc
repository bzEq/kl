// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "kl/hexdump.h"
#include "kl/string.h"

namespace kl {

void HexDump(const Slice &s, std::string *result) {
  char buf[4];
  for (size_t i = 0; i < s.len; ++i) {
    if (i) {
      result->append(" ");
    }
    snprintf(buf, sizeof(buf), "%02x", static_cast<unsigned char>(s.data[i]));
    result->append(buf);
  }
}

}  // namespace kl
