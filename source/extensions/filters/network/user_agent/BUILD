licenses(["notice"])  # Apache 2

# user_agent L4 network filter. This is primarily a simplistic example.
# Public docs: docs/root/configuration/network_filters/user_agent_filter.rst

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_extension",
    "envoy_cc_library",
    "envoy_package",
)

envoy_package()

envoy_cc_library(
    name = "user_agent",
    srcs = ["user_agent.cc"],
    hdrs = ["user_agent.h"],
    deps = [
        "//include/envoy/buffer:buffer_interface",
        "//include/envoy/network:connection_interface",
        "//include/envoy/network:filter_interface",
        "//source/common/common:assert_lib",
        "//source/common/common:minimal_logger_lib",
        "@envoy_api//envoy/extensions/filters/network/user_agent/v3:pkg_cc_proto",
    ],
)

envoy_cc_extension(
    name = "config",
    srcs = ["config.cc"],
    hdrs = ["config.h"],
    security_posture = "unknown",
    deps = [
        ":user_agent",
        "//include/envoy/registry",
        "//include/envoy/server:filter_config_interface",
        "//source/extensions/filters/network:well_known_names",
        "//source/extensions/filters/network/common:factory_base_lib",
        "@envoy_api//envoy/extensions/filters/network/user_agent/v3:pkg_cc_proto",
    ],
)
