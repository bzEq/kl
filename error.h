// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.

#ifndef KL_ERROR_H_
#define KL_ERROR_H_
#include <string>

#include "result.h"

namespace kl {

enum {
  kGeneralErrCode = -1,
};

struct Error {
  int32_t code;
  std::string message;
  int32_t Code() const { return code; }
  const std::string &ToString() const { return message; }
  const char *ToCString() const { return message.c_str(); }
  std::string &&MoveString() { return std::move(message); }
};

template <typename T>
using Result = result::Result<T, Error>;

template <typename T>
inline auto Ok(T &&v) {
  return result::Ok(std::forward<T>(v));
}

inline auto Ok() { return result::Ok(); }

inline auto Err(Error &&err) { return result::Err(std::forward<Error>(err)); }

inline auto Err(int32_t code, std::string &&msg) {
  return result::Err(Error{
      .code = code, .message = std::forward<std::string>(msg),
  });
}

inline auto Err(std::string &&msg) {
  return Err(kGeneralErrCode, std::forward<std::string>(msg));
}

inline auto Err(int32_t code, const char *msg) {
  return result::Err(Error{
      .code = code, .message = std::string(msg),
  });
}

inline auto Err(const char *msg) { return Err(kGeneralErrCode, msg); }

template <typename... Args>
inline auto FormatString(const char *fmt, Args &&... args) {
  static const size_t init_size = 256;
  static const size_t size_limit = 2048;
  char *buf = new char[init_size];
  int total = std::snprintf(buf, init_size, fmt, std::forward<Args>(args)...);
  if (total > init_size) {
    delete[] buf;
    buf = new char[size_limit];
    std::snprintf(buf, size_limit, fmt, std::forward<Args>(args)...);
  }
  std::string msg(buf);
  delete[] buf;
  return msg;
}

template <typename... Args>
inline auto Err(int32_t code, const char *fmt, Args &&... args) {
  return Err(code, FormatString(fmt, std::forward<Args>(args)...));
}

template <typename... Args>
inline auto Err(const char *fmt, Args &&... args) {
  return Err(FormatString(fmt, std::forward<Args>(args)...));
}

}  // namespace kl
#endif
