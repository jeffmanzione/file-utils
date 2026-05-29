load("@rules_cc//cc:cc_test.bzl", "cc_test")
load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "string_utils",
    srcs = ["string_utils.c"],
    hdrs = ["string_utils.h"],
)

cc_test(
    name = "string_utils_test",
    size = "small",
    srcs = ["string_utils_test.cc"],
    deps = [
        ":string_utils",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
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

cc_test(
    name = "file_info_test",
    size = "small",
    srcs = ["file_info_test.cc"],
    deps = [
        ":file_info",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
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

cc_test(
    name = "file_utils_test",
    size = "small",
    srcs = ["file_utils_test.cc"],
    deps = [
        ":file_utils",
        "@abseil-cpp//absl/strings:str_format",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "sfile",
    srcs = ["sfile.c"],
    hdrs = ["sfile.h"],
)

cc_test(
    name = "sfile_test",
    size = "small",
    srcs = ["sfile_test.cc"],
    deps = [
        ":sfile",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
    ],
)
