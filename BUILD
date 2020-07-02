load("@rules_cc//cc:defs.bzl", "cc_library")
load("@rules_proto//proto:defs.bzl", "proto_library")


# Convention:
# A cc_proto_library that wraps a proto_library named foo_proto
# should be called foo_cc_proto.
# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary
cc_binary(
    name = "person_cc",
    deps = ["person_lib"],
)
cc_library(
    name = "person_lib",
    srcs = ["read_write.cc"],
    deps = [
        "person_cc_proto",
        "@com_google_absl//absl/strings:strings",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)

cc_binary(
    name = "repeated_cc",
    deps = ["repeated_lib"],
)
cc_library(
    name = "repeated_lib",
    srcs = ["repeated.cc"],
    deps = [
        "person_cc_proto",
        "@com_google_absl//absl/strings:strings",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)


cc_proto_library(
    name = "person_cc_proto",
    deps = [":person_proto",]
)

# Conventions:
# 1. One proto_library rule per .proto file.
# 2. A file named foo.proto will be in a rule named foo_proto.
proto_library(
    name = "person_proto",
    srcs = ["person.proto"],
)