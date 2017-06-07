// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_HEXDUMP_H_
#define KL_HEXDUMP_H_

#include <string>

#include "kl/slice.h"

namespace kl {

void HexDump(const Slice &s, std::string *result);

}  // namespace kl
#endif  // KL_HEXDUMP_H_
