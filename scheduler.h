// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

// Just another multi-thread executor.

#ifndef KL_SCHEDULER_H_
#define KL_SCHEDULER_H_
#include <atomic>
#include <cstdint>
#include <map>
#include <mutex>
#include <string>

#include "chan.h"
#include "epoll.h"
#include "error.h"
#include "logger.h"
#include "rwlock.h"
#include "wait_group.h"

namespace kl {

class Scheduler {
public:
  typedef std::function<void(uint32_t events)> EpollHandler;
  explicit Scheduler(size_t num_of_worker_threads);
  Scheduler(size_t num_of_worker_threads, bool enable_epoll);
  kl::Status Go();
  kl::Status RegisterEpollEvent(int fd, uint32_t events,
                                EpollHandler &&callback);
  kl::Status UnregisterEpollEvent(int fd);
  kl::logging::Logger &Logger();
  void SubmitTask(std::function<void(void)> &&task);
  void Stop(const char *reason);
  ~Scheduler();

private:
  size_t PickQueue();
  kl::Status LaunchLoggingThread();
  kl::Status LaunchEpollThread();
  kl::Status LaunchWorkerThreads();
  void WorkerRoutine(size_t id);
  std::unique_ptr<kl::logging::Logger> logger_;
  std::mutex exit_reason_mutex_;
  std::string exit_reason_;
  kl::Chan<std::string> logging_queue_;
  std::map<int, EpollHandler> callbacks_;
  kl::rwlock::Lock callback_mapping_rwlock_;
  kl::WaitGroup sync_;
  kl::Epoll epoll_;
  size_t num_of_worker_threads_;
  bool enable_epoll_;
  std::atomic<bool> stop_;
  kl::Chan<std::function<void(void)>> *task_queues_;
  std::atomic<size_t> queue_round_robin_;
};

}  // namespace kl
#endif
