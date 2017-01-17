// Copyright (c) 2017 Kai Luo <gluokai@gmail.com>. All rights reserved.
// Use of this source code is governed by the BSD license that can be found in
// the LICENSE file.

#include "scheduler.h"
#include "env.h"

namespace kl {

namespace {
const int kLoggingThreadWaitTimeout = 5000;
const int kEpollThreadWaitTimeout = 5000;
const int kWorkerThreadWaitTimeout = 5000;
}

// REQUIRES: num_of_worker_threads >= 1
Scheduler::Scheduler(size_t num_of_worker_threads)
    : num_of_worker_threads_(num_of_worker_threads), stop_(false),
      task_queues_(nullptr), queue_round_robin_(0) {}

Scheduler::~Scheduler() {
  if (task_queues_) {
    delete[] task_queues_;
    task_queues_ = nullptr;
  }
}

kl::Status Scheduler::LaunchLoggingThread() {
  logger_ = std::make_unique<kl::logging::Logger>([this](const char *message) {
    logging_queue_.Push(std::string(message));
  });
  sync_.Add();
  std::thread([this] {
    kl::env::Defer defer([this] { sync_.Done(); });
    while (!stop_) {
      auto next = logging_queue_.Pop(kLoggingThreadWaitTimeout);
      if (!next && logging_queue_.Closed()) {
        break;
      }
      if (next) {
        std::string &message = *next;
        std::fprintf(stderr, "%s", message.c_str());
      }
    }
  }).detach();
  return kl::Ok();
}

kl::Status Scheduler::RegisterEpollEvent(int fd, uint32_t events,
                                         EpollHandler &&callback) {
  auto status = epoll_.AddFd(fd, events);
  if (!status) {
    return status;
  }
  callback_mapping_rwlock_.WLock();
  callbacks_.insert(std::make_pair(fd, std::move(callback)));
  callback_mapping_rwlock_.WUnlock();
  return kl::Ok();
}

kl::Status Scheduler::UnregisterEpollEvent(int fd) {
  auto status = epoll_.DelFd(fd);
  if (!status) {
    return status;
  }
  callback_mapping_rwlock_.WLock();
  callbacks_.erase(fd);
  callback_mapping_rwlock_.WUnlock();
  return kl::Ok();
}

void Scheduler::SubmitTask(std::function<void(void)> &&task) {
  size_t current = queue_round_robin_++;
  current = current % num_of_worker_threads_;
  task_queues_[current].Push(std::move(task));
}

kl::Status Scheduler::LaunchEpollThread() {
  sync_.Add();
  std::thread([this] {
    kl::env::Defer defer([this] { sync_.Done(); });
    while (!stop_) {
      auto wait = epoll_.Wait(64, kEpollThreadWaitTimeout);
      if (!wait) {
        Stop(wait.Err().ToCString());
        break;
      }
      for (const auto &event : *wait) {
        int fd = event.data.fd;
        uint32_t events = event.events;
        kl::rwlock::RDGuard _(&callback_mapping_rwlock_);
        auto iter = callbacks_.find(fd);
        if (iter == callbacks_.end()) {
          KL_DEBUG_L(Logger(), "callback for fd %d is empty");
          continue;
        }
        SubmitTask([ handler = iter->second, events ] { handler(events); });
      }
    }
  }).detach();
  return kl::Ok();
}

void Scheduler::WorkerRoutine(size_t id) {
  kl::env::Defer defer([this] { sync_.Done(); });
  while (!stop_) {
    auto task = task_queues_[id].Pop(kWorkerThreadWaitTimeout);
    if (!task && task_queues_[id].Closed()) {
      break;
    }
    if (task) {
      try {
        (*task)();
      } catch (const std::exception &e) {
        KL_ERROR_L(Logger(), e.what());
      }
    }
  }
}

kl::logging::Logger &Scheduler::Logger() {
  assert(logger_);
  return *logger_;
}

kl::Status Scheduler::Go() {
  auto status = LaunchLoggingThread();
  if (!status) {
    Stop(status.Err().ToCString());
    return status;
  }
  status = LaunchEpollThread();
  if (!status) {
    Stop(status.Err().ToCString());
    return status;
  }
  status = LaunchWorkerThreads();
  if (!status) {
    Stop(status.Err().ToCString());
    return status;
  }
  sync_.Wait();
  if (!exit_reason_.empty()) {
    return kl::Err(exit_reason_.c_str());
  }
  return kl::Ok();
}

void Scheduler::Stop(const char *reason) {
  if (reason) {
    exit_reason_ = reason;
  }
  stop_.store(true);
}

kl::Status Scheduler::LaunchWorkerThreads() {
  task_queues_ =
      new kl::Chan<std::function<void(void)>>[num_of_worker_threads_];
  for (size_t i = 0; i < num_of_worker_threads_; ++i) {
    sync_.Add();
    std::thread([this, i] { WorkerRoutine(i); }).detach();
  }
  return kl::Ok();
}

}  // namespace kl
