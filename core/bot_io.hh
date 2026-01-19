#pragma once

#include <functional>
#include <sstream>
#include <map>
#include <string_view>
#include <format>
#include <vector>


struct OutputFormat {
  bool table = false;
};

class BotOutputProtocol {
 public:
  virtual void write_message(std::string_view msg) = 0;
  virtual void write_message(std::string_view msg, const OutputFormat & format) {
    write_message(msg);
  }
  virtual ~BotOutputProtocol() {}
};

class User {
 public:
  virtual std::string get_nickname() const = 0;
  virtual std::string get_mention() const = 0;
  virtual ~User() {}
};

enum class BotStatus {
  OK = 0,
  COMMAND_NOT_FOUND = 1,
  FAILED = 2
};

struct CommandHandler {
  using HandlerType = std::function<bool(std::istringstream &, const User &, BotOutputProtocol &)>;
  HandlerType handler;
  std::string help;
};

struct CommandInfo {
  std::string command;
  std::string help;
};

class Bot {
 public:

  BotStatus handle_msg(std::string_view msg, const User & user, BotOutputProtocol & out) { 
    std::istringstream ss{std::string(msg)};
    std::string command;
    ss >> command;
    auto handler = _handlers.find(command);
    if (handler == _handlers.end()) {
      out.write_message(std::format("Command {} not found.", command));
      auto list = short_command_list(5);
      if (!list.empty()) {
        out.write_message(std::format("Available commands: {}", list));
      }
      return BotStatus::COMMAND_NOT_FOUND;
    }
    if (!handler->second.handler(ss, user, out)) {
      return BotStatus::FAILED;
    }
    return BotStatus::OK;
  }

  void register_handler(std::string_view command, std::string_view help, CommandHandler::HandlerType h) {
    _handlers[std::string(command)] = CommandHandler{
      .handler = h,
      .help = std::string(help)
    }; //TODO: Better insert plz.
  }

  std::vector<CommandInfo> list_handlers() const {
    std::vector<CommandInfo> result;
    result.reserve(_handlers.size());
    for (const auto & [name, handler] : _handlers) {
      result.push_back(CommandInfo{.command = name, .help = handler.help});
    }
    return result;
  }

  std::string short_command_list(std::size_t max_count) const {
    std::string result;
    std::size_t count = 0;
    for (const auto & [name, handler] : _handlers) {
      if (max_count > 0 && count >= max_count) {
        break;
      }
      if (count > 0) {
        result += ", ";
      }
      result += name;
      ++count;
    }
    if (max_count > 0 && _handlers.size() > max_count) {
      result += std::format(", ... ({} total)", _handlers.size());
    }
    return result;
  }

  virtual ~Bot() = default;
 protected:
  std::map<std::string, CommandHandler> _handlers;
};
