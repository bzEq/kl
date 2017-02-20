// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_ERROR_H_
#define KL_ERROR_H_
#include <cstring>
#include <string>
#include <vector>

#include "result.h"
#include "string.h"

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

template <typename T, typename... Args>
inline auto MakeResult(Args &&... args) {
  return result::MakeResult<T>(std::forward<Args>(args)...);
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
inline auto Err(int32_t code, const char *fmt, Args &&... args) {
  return Err(code, string::FormatString(fmt, std::forward<Args>(args)...));
}

template <typename... Args>
inline auto Err(const char *fmt, Args &&... args) {
  return Err(string::FormatString(fmt, std::forward<Args>(args)...));
}

using Status = Result<void>;

}  // namespace kl
#endif
