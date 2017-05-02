cc_test(
    name = "kl_test",
    srcs = glob(["*_test.cc"]) + ["main.cc"],
    copts = ["-std=c++14", "-Wall", "-Werror",
             "-g", "-Iinclude"],
    linkopts = ["-lpthread"],
    deps = ["//:kl"],
)
