package(default_visibility = ["//visibility:public"])


cc_library(
  name = "character_sheet",
  srcs = ["character_sheet.cc"],
  hdrs = ["character_sheet.hh"],
  deps = [
    "//:calculator",
    "//:utils",
    "@dpp//:nlohmann_json",
  ],
)

cc_library(
  name = "user_input",
  srcs = ["user_input.cc"],
  hdrs = ["user_input.hh"],
  deps = [
    ":character_sheet",
  ],
)

cc_binary(
  name = "make_characters",
  srcs = ["make_characters.cc"],
  deps = [
    "//:config",
    ":character_sheet",
    "@dpp//:nlohmann_json",
  ],
)

cc_test(
  name = "test",
  srcs = ["test.cc"],
  deps = [
    "//:utils",
    ":character_sheet",
  ],
)
