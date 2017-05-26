cc_library(
    licenses = ["notice"],
    visibility = ["//visibility:public"],
    name = "kl",
    srcs = glob(["*.cc"]),
    hdrs = glob(["include/kl/*.h"]),
    includes = ["include"],
    copts = [
        "-std=c++14",
        "-Wall",
        "-Werror",
    ],
    linkopts = ["-lpthread"],
)
