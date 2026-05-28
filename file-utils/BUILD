load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "string_utils",
    srcs = ["string_utils.c"],
    hdrs = ["string_utils.h"],
)

cc_library(
    name = "file_info",
    srcs = ["file_info.c"],
    hdrs = ["file_info.h"],
    deps = [
        ":file_utils",
        ":sfile",
    ],
)

cc_library(
    name = "file_utils",
    srcs = ["file_utils.c"],
    hdrs = ["file_utils.h"],
    deps = [
        "//file-utils:string_utils",
    ],
)

cc_library(
    name = "sfile",
    srcs = ["sfile.c"],
    hdrs = ["sfile.h"],
)
