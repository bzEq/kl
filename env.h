// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#ifndef KL_ENV_H_
#define KL_ENV_H_
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstring>

#include <atomic>
#include <functional>
#include <map>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "error.h"
#include "random.h"

namespace kl {
namespace env {

// A {thread,dependency}-safe initialize wrapper
template <typename T>
class SafeInitializer {
public:
  SafeInitializer() : store_(nullptr) {}

  template <typename... Args>
  T *InitAndGet(Args &&... args) {
    static T v(std::forward<Args>(args)...);
    store_ = &v;
    return store_;
  }

  bool Inited() { return store_ != nullptr; }

  T *Get() { return store_; }

private:
  std::atomic<T *> store_;
};

class ArgvBuilder {
public:
  std::vector<char *> Build() {
    std::vector<char *> result;
    for (auto &arg : argv_) {
      result.push_back(&arg[0]);
    }
    return result;
  }

  void Append(const std::string &arg) { argv_.push_back(arg); }

  void Append(std::string &&arg) { argv_.push_back(std::move(arg)); }

private:
  std::vector<std::string> argv_;
};

class EnvpBuilder {
public:
  std::vector<char *> Build() {
    build_store_.clear();
    for (auto &iter : env_) {
      std::string kv;
      kv.append(iter.first);
      kv.append("=");
      kv.append(iter.second);
      build_store_.push_back(kv);
    }
    std::vector<char *> result;
    for (auto &kv : build_store_) {
      result.push_back(&kv[0]);
    }
    return result;
  }

  void Put(const std::string &key, const std::string &value) {
    env_.insert(std::make_pair(key, value));
  }

private:
  std::map<std::string, std::string> env_;
  std::vector<std::string> build_store_;
};

class Defer {
public:
  Defer() = default;
  Defer(const Defer &) = delete;
  Defer(Defer &&) = default;
  explicit Defer(std::function<void(void)> &&f) { defer.push(std::move(f)); }
  Defer &operator()(std::function<void(void)> &&f) {
    defer.push(std::move(f));
    return *this;
  }
  ~Defer() {
    while (!defer.empty()) {
      auto f = std::move(defer.top());
      defer.pop();
      f();
    }
  }

private:
  std::stack<std::function<void(void)>> defer;
};

inline bool FileExists(const char *file) {
  struct stat buff;
  if (stat(file, &buff) == 0) {
    return true;
  }
  return errno != ENOENT;
}

inline Result<int64_t> FileSize(const char *file) {
  struct stat buff;
  if (stat(file, &buff) != 0) {
    return Err(std::strerror(errno));
  }
  return Ok(buff.st_size);
}

inline Status TruncateFile(const char *file, off_t len) {
  int ret = ::truncate(file, len);
  if (ret != 0) {
    return Err(std::strerror(errno));
  }
  return Ok();
}

inline Result<std::string> ReadFile(const char *file) {
  int fd = ::open(file, O_RDONLY);
  if (fd < 0) {
    return Err(errno, std::strerror(errno));
  }
  Defer defer([fd]() { ::close(fd); });
  struct stat buff;
  int err = fstat(fd, &buff);
  if (err < 0) {
    return Err(errno, std::strerror(errno));
  }
  off_t fsize = buff.st_size;
  std::string data;
  if (fsize == 0) {
    return Ok(data);
  }
  data.resize(fsize);
  ssize_t n = ::read(fd, static_cast<void *>(&data[0]), fsize);
  if (n != fsize) {
    return Err(std::strerror(errno));
  }
  return Ok(std::move(data));
}

inline Status FillFileWith(const char *file, const std::string &data) {
  int fd = ::open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
  if (fd < 0) {
    return Err(std::strerror(errno));
  }
  Defer defer([fd]() { ::close(fd); });
  ssize_t n = ::write(fd, static_cast<const void *>(data.c_str()), data.size());
  if (n != static_cast<ssize_t>(data.size())) {
    return Err(std::strerror(errno));
  }
  n = ::fsync(fd);
  if (n < 0) {
    return Err(std::strerror(errno));
  }
  return Ok();
}

inline Status CreateEmptyFile(const char *file) {
  return FillFileWith(file, "");
}

inline void MemoryBarrier() {
  // https://en.wikipedia.org/wiki/Memory_ordering
  asm volatile("" ::: "memory");
}

inline Status DeleteFile(const char *file) {
  if (::unlink(file) != 0 && errno != ENOENT) {
    return Err(errno, std::strerror(errno));
  }
  return Ok();
}

inline Status RenameFile(const char *oldpath, const char *newpath) {
  if (::rename(oldpath, newpath) != 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok();
}

inline Status CreateDir(const char *dir) {
  if (::mkdir(dir, 0755) != 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok();
}

inline Status DeleteDir(const char *dir) {
  if (::rmdir(dir) != 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok();
}

inline Status SetNonBlocking(int fd) {
  int flags = ::fcntl(fd, F_GETFL, 0);
  if (flags < 0) {
    return Err(errno, std::strerror(errno));
  }
  int err = ::fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  if (err < 0) {
    return Err(errno, std::strerror(errno));
  }
  return Ok();
}

// Something like emacs' make-temp-name
inline std::string MakeTempName(const char *prefix) {
  std::string name(prefix);
  name += std::to_string(::getpid());
  for (int i = 0; i < 3; ++i) {
    name.push_back(random::RandomPrintableChar());
  }
  return name;
}

// Something like emacs' make-temp-file
// RETURNS: path of the temp file
inline Result<std::string> MakeTempFile(const char *prefix) {
  static const std::string temp_root("/tmp/");
  std::string filename(temp_root + MakeTempName(prefix));
  auto status = CreateEmptyFile(filename.c_str());
  if (!status) {
    return kl::Err(status.MoveErr());
  }
  return kl::Ok(std::move(filename));
}

inline Result<std::vector<std::string>> GetChildren(const char *dir) {
  DIR *d = ::opendir(dir);
  if (d == nullptr) {
    return kl::Err(errno, std::strerror(errno));
  }
  Defer defer([d] { ::closedir(d); });
  std::vector<std::string> result;
  struct dirent *entry;
  while ((entry = ::readdir(d)) != nullptr) {
    result.push_back(entry->d_name);
  }
  return kl::Ok(std::move(result));
}

inline bool IsDir(const char *dir) {
  struct stat buf;
  int err = ::lstat(dir, &buf);
  if (err < 0 || !S_ISDIR(buf.st_mode)) {
    return false;
  }
  return true;
}

inline bool IsRegularFile(const char *fname) {
  struct stat buf;
  int err = ::lstat(fname, &buf);
  if (err < 0 || !S_ISREG(buf.st_mode)) {
    return false;
  }
  return true;
}

inline Status WritePidToFile(const char *fname) {
  // This function is always successful.
  pid_t pid = getpid();
  std::string s = std::to_string(pid);
  return FillFileWith(fname, s);
}

}  // namespace env
}  // namespace kl
#endif
