// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
#ifndef KL_LOGGER_H_
#define KL_LOGGER_H_
#include <cassert>
#include <functional>
#include <memory>

namespace kl {
namespace logging {

enum LogLevel {
  kInfo = 0,
  kDebug,
  kWarn,
  kError,
  kFatal,
};

extern const char *kLogLevelString[5];

class Logger {
public:
  static Logger &DefaultLogger();
  static void SetDefaultLogger(Logger &&logger);

  Logger() = delete;
  Logger(const Logger &) = delete;
  Logger(Logger &&) = default;
  explicit Logger(std::function<void(const char *)> &&output);
  Logger(int log_level, std::function<void(const char *)> &&output);
  void SetLogLevel(int log_level);
  void Logging(int log_level, const char *file, const char *func, int line,
               const char *fmt, ...);

private:
  void Logging(const char *file, const char *func, int line, const char *fmt,
               va_list ap);

  static std::unique_ptr<Logger> default_logger_;
  // static Logger *default_logger_;
  int log_level_;
  std::function<void(const char *)> output_;
};
}  // namespace logging
}  // namespace kl

#define KL_INFO(...)                                                           \
  kl::logging::Logger::DefaultLogger().Logging(                                \
      kl::logging::kInfo, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define KL_DEBUG(...)                                                          \
  kl::logging::Logger::DefaultLogger().Logging(                                \
      kl::logging::kDebug, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define KL_WARN(...)                                                           \
  kl::logging::Logger::DefaultLogger().Logging(                                \
      kl::logging::kWarn, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define KL_ERROR(...)                                                          \
  kl::logging::Logger::DefaultLogger().Logging(                                \
      kl::logging::kError, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define KL_FATAL(...)                                                          \
  kl::logging::Logger::DefaultLogger().Logging(                                \
      kl::logging::kFatal, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define KL_INFO_L(logger, ...)                                                 \
  (logger).Logging(kl::logging::kInfo, __FILE__, __FUNCTION__, __LINE__,       \
                   ##__VA_ARGS__)
#define KL_DEBUG_L(logger, ...)                                                \
  (logger).Logging(kl::logging::kDebug, __FILE__, __FUNCTION__, __LINE__,      \
                   ##__VA_ARGS__)
#define KL_WARN_L(logger, ...)                                                 \
  (logger).Logging(kl::logging::kWarn, __FILE__, __FUNCTION__, __LINE__,       \
                   ##__VA_ARGS__)
#define KL_ERROR_L(logger, ...)                                                \
  (logger).Logging(kl::logging::kError, __FILE__, __FUNCTION__, __LINE__,      \
                   ##__VA_ARGS__)
#define KL_FATAL_L(logger, ...)                                                \
  (logger).Logging(kl::logging::kFatal, __FILE__, __FUNCTION__, __LINE__,      \
                   ##__VA_ARGS__)

#endif
