cc_library(
    visibility = ["//visibility:public"],
    name = "libkl",
    srcs = ["buffer.cc", "epoll.cc",
            "lex_buffer.cc", "logger.cc",
            "scheduler.cc", "states.cc", "testkit.cc",
            "utf8.cc"],
    hdrs = ["any.h", "bitset.h", "buffer.h",
            "chan.h", "coding.h", "env.h", "epoll.h",
            "error.h", "event_order.h", "filelock.h",
            "hash.h", "heap.h", "inet.h", "lex_buffer.h",
            "logger.h", "netdev.h", "object_pool.h",
            "option.h", "random.h", "result.h", "rwlock.h",
            "scheduler.h", "slice.h", "states.h", "string.h",
            "subprocess.h", "tcp.h", "testkit.h", "timer.h", "udp.h",
            "utf8.h", "wait_group.h"],
    copts = ["-std=c++14", "-Wall", "-Werror",
             "-g"],
    linkopts = ["-lpthread"],
)
