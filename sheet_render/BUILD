load("@rules_python//python:defs.bzl", "py_binary", "py_library", "py_test")
load("@pip//:requirements.bzl", "requirement")

py_library(
    name = "sheet_render_lib",
    srcs = [
        "__init__.py",
        "render.py",
    ],
    data = ["template.j2"],
    deps = [requirement("jinja2")],
)

py_binary(
    name = "sheet_render_cli",
    srcs = ["sheet_render_cli.py"],
    deps = [
        ":sheet_render_lib",
        requirement("jinja2"),
    ],
)

py_test(
    name = "render_test",
    srcs = ["render_test.py"],
    deps = [":sheet_render_lib"],
)
