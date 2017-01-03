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
    void Run();                                                                \
    static void RunIt() {                                                      \
      TOKCAT(name, Test) t;                                                    \
      t.Run();                                                                 \
    }                                                                          \
  };                                                                           \
  bool TOKCAT(name, TestResult) =                                              \
      kl::test::RegisterTest(#base, #name, &TOKCAT(name, Test)::RunIt);        \
  void TOKCAT(name, Test)::Run()

template <typename T>
inline void Assert(const char *file, int line, const char *expr, T &&v) {
  if (!std::forward<T>(v)) {
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
