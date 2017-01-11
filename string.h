// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#ifndef KL_STRING_H_
#define KL_STRING_H_
#include <cstring>
#include <memory>
#include <string>
#include <vector>

namespace kl {
namespace string {

template <typename... Args>
inline auto FormatString(const char *fmt, Args &&... args) {
  static const size_t init_size = 256;
  static const size_t size_limit = 2048;
  char *buf = new char[init_size];
  int total = std::snprintf(buf, init_size, fmt, std::forward<Args>(args)...);
  if (total > static_cast<int>(init_size)) {
    delete[] buf;
    buf = new char[size_limit];
    std::snprintf(buf, size_limit, fmt, std::forward<Args>(args)...);
  }
  std::string msg(buf);
  delete[] buf;
  return msg;
}

inline std::vector<std::string> SplitString(const char *str,
                                            const char *delim) {
  size_t size = ::strlen(str) + 1;
  auto buf = std::unique_ptr<char[]>(new char[size]);
  ::strncpy(buf.get(), str, size);
  char *save = buf.get();
  std::vector<std::string> result;
  while (true) {
    char *s = ::strtok_r(save, delim, &save);
    if (s == nullptr) {
      break;
    }
    result.push_back(std::string(s));
  }
  return result;
}

}  // namespace string
}  // namespace kl
#endif
