// Copyright (c) 2011 The LevelDB Authors. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:

//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.

#ifndef KL_TESTKIT_H_
#define KL_TESTKIT_H_
#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <string>

namespace kl {
namespace test {

extern int RunAllTests();
extern bool RegisterTest(const char *base, const char *name,
                         void (*func)(void));

#define TOKCAT(a, b) a##b
#define TEST(base, name, ...)                                                  \
  class TOKCAT(name, Test) : public base {                                     \
  public:                                                                      \
    TOKCAT(name, Test)() : base(__VA_ARGS__) {}                                \
    void _Run();                                                               \
    static void _RunIt() {                                                     \
      TOKCAT(name, Test) t;                                                    \
      t._Run();                                                                \
    }                                                                          \
  };                                                                           \
  bool TOKCAT(name, TestResult) =                                              \
      kl::test::RegisterTest(#base, #name, &TOKCAT(name, Test)::_RunIt);       \
  void TOKCAT(name, Test)::_Run()

template <typename T>
inline void Assert(const char *file, int line, const char *expr, T &&v) {
  if (!v) {
    char buf[1024];
    std::snprintf(buf, sizeof(buf), "[%s:%d] assert expression `%s' failed",
                  file, line, expr);
    throw std::runtime_error(buf);
  }
}

}  // namespace test
}  // namespace kl

#define ASSERT(expr) kl::test::Assert(__FILE__, __LINE__, #expr, (expr))
#define KL_TEST kl::test::RunAllTests

#endif
