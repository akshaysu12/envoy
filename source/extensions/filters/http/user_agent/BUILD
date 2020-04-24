licenses(["notice"])  # Apache 2

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_extension",
    "envoy_cc_library",
    "envoy_package",
)

envoy_package()

envoy_cc_extension(
    name = "config",
    srcs = ["config.cc"],
    hdrs = ["config.h"],
    security_posture = "robust_to_untrusted_downstream",
    deps = [
        "//include/envoy/registry",
        "//source/extensions/filters/http:well_known_names",
        "//source/extensions/filters/http/common:factory_base_lib",
        "//source/extensions/filters/http/user_agent:user_agent_lib",
        "@envoy_api//envoy/extensions/filters/http/user_agent/v3:pkg_cc_proto",
    ],
)

envoy_cc_library(
    name = "user_agent_lib",
    srcs = ["user_agent_filter.cc"],
    hdrs = ["user_agent_filter.h"],
    deps = [
        "//include/envoy/http:filter_interface",
        "//include/envoy/stats:stats_macros",
        "//source/common/stats:symbol_table_lib",
        "//source/common/http:utility_lib",
        "//source/extensions/filters/http:well_known_names",
        "@envoy_api//envoy/extensions/filters/http/user_agent/v3:pkg_cc_proto",
    ],
)
