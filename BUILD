package(default_visibility = ["//visibility:public"])

cc_library(
  name = "tokens",
  srcs = ["tokens.cc"],
  hdrs = ["tokens.hh"],
)

cc_library(
  name = "rpn",
  srcs = ["rpn.cc"],
  hdrs = ["rpn.hh"],
  deps = [
    ":tokens",
  ],
)

cc_library(
  name = "config",
  srcs = ["config.cc"],
  hdrs = ["config.hh"],
  deps = [
    "@dpp//:nlohmann_json"
  ],
)

cc_library(
  name = "commands",
  srcs = ["commands.cc"],
  hdrs = ["commands.hh"],
  deps = [
    "//cthulhu:user_input",
    "//cthulhu:character_sheet",
    ":calculator",
    "//core:bot_io",
  ],
)

cc_library(
  name = "bot_setup",
  srcs = ["bot_setup.cc"],
  hdrs = ["bot_setup.hh"],
  deps = [
    ":commands",
    "//core:bot_io",
    "//cthulhu:character_sheet",
  ],
)

cc_library(
  name = "discord_commands",
  srcs = ["discord_commands.cc"],
  hdrs = ["discord_commands.hh"],
  deps = [
    "//cthulhu:user_input",
    "//cthulhu:character_sheet",
    ":calculator",
    "@dpp//:dpp",
  ],
)

cc_library(
  name = "utils",
  hdrs = ["utils.hh"],
)

cc_library(
  name = "calculator",
  srcs = ["calculator.cc"],
  hdrs = ["calculator.hh"],
  deps = [
    ":tokens",
    ":rpn",
    ":utils",
  ],
)

cc_binary(
  name = "bot",
  srcs = ["bot.cpp", "bot.h"],
  deps = [
    ":bot_setup",
    ":config",
    "//discord:discord_bot",
    ":calculator",
    "//core:bot_io",
    "//cthulhu:user_input",
    "//cthulhu:character_sheet",
    "@dpp//:dpp",
    "@dpp//:nlohmann_json",
  ],
)
