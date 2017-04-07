cc_test(
    name = "libkl-test",
    srcs = glob(["*_test.cc"]) + ["main.cc"],
    copts = ["-std=c++14", "-Wall", "-Werror",
             "-g"],
    linkopts = ["-lpthread"],
    deps = ["//:libkl"],
)
