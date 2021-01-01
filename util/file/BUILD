load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "file_info",
    srcs = ["file_info.c"],
    hdrs = ["file_info.h"],
    deps = [
        ":file_util",
        ":sfile",
        "@memory_wrapper//alloc",
        "@memory_wrapper//debug",
    ],
)

cc_library(
    name = "file_util",
    srcs = ["file_util.c"],
    hdrs = ["file_util.h"],
    deps = [
        "//util:string",
        "@memory_wrapper//alloc",
        "@memory_wrapper//alloc/arena:intern",
        "@memory_wrapper//debug",
    ],
)

cc_library(
    name = "sfile",
    srcs = ["sfile.c"],
    hdrs = ["sfile.h"],
    deps = [
        "@memory_wrapper//alloc",
    ],
)
