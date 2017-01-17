// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.
#include <sys/time.h>

#include "logger.h"

namespace kl {
namespace logging {
const char *kLogLevelString[5] = {"INFO", "DEBUG", "WARN", "ERROR", "FATAL"};

Logger::Logger(std::function<void(const char *)> &&output)
    : log_level_(kInfo), output_(std::move(output)) {}
Logger::Logger(int log_level, std::function<void(const char *)> &&output)
    : log_level_(log_level), output_(std::move(output)) {}
std::unique_ptr<Logger> Logger::default_logger_(new Logger([](const char *msg) {
  std::fprintf(stderr, "%s\n", msg);
}));

void Logger::Logging(int log_level, const char *file, const char *func,
                     int line, const char *fmt, ...) {
  if (log_level < log_level_) {
    return;
  }
  va_list ap;
  va_start(ap, fmt);
  Logging(log_level, file, func, line, fmt, ap);
  va_end(ap);
}

void Logger::Logging(int log_level, const char *file, const char *func,
                     int line, const char *fmt, va_list ap) {
  if (log_level < log_level_) {
    return;
  }
  static const char *kPrefixFormat =
      "[%s %04d/%02d/%02d-%02d:%02d:%02d.%06ld %s:%s:%d] ";
  struct timeval now;
  ::gettimeofday(&now, nullptr);
  struct tm t;
  ::localtime_r(&now.tv_sec, &t);
  int prefix_size =
      std::snprintf(nullptr, 0, kPrefixFormat, kLogLevelString[log_level],
                    t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour,
                    t.tm_min, t.tm_sec, now.tv_usec, file, func, line);
  va_list backup_ap;
  va_copy(backup_ap, ap);
  int msg_size = std::vsnprintf(nullptr, 0, fmt, backup_ap);
  va_end(backup_ap);

  int buf_size = prefix_size + msg_size + 1;
  char *buf = new char[buf_size];
  const char *base = buf;
  prefix_size =
      std::snprintf(buf, buf_size, kPrefixFormat, kLogLevelString[log_level],
                    t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour,
                    t.tm_min, t.tm_sec, now.tv_usec, file, func, line);
  buf += prefix_size;
  assert(buf_size >= prefix_size);
  std::vsnprintf(buf, buf_size - prefix_size, fmt, ap);
  output_(base);
  delete[] base;
}

Logger &Logger::DefaultLogger() {
  assert(default_logger_);
  return *default_logger_;
}

void Logger::SetDefaultLogger(Logger &&logger) {
  default_logger_ = std::make_unique<Logger>(std::move(logger));
}

void Logger::SetLogLevel(int log_level) { log_level_ = log_level; }

}  // namespace logging
}  // namespace kl
