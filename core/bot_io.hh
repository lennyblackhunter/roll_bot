#pragma once

#include <functional>
#include <sstream>
#include <map>
#include <string_view>
#include <format>


class BotOutputProtocol {
 public:
  virtual void write_message(std::string_view msg) = 0;
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

class Bot {
 public:
  using HandlerType = std::function<bool(std::istringstream &, const User &, BotOutputProtocol &)>;

  BotStatus handle_msg(std::string_view msg, const User & user) { 
    std::istringstream ss{std::string(msg)};
    std::string command;
    ss >> command;
    auto handler = _handlers.find(command);
    if (handler != _handlers.end()) {
      get_output().write_message(std::format("Command {} not found.", command));
      return BotStatus::COMMAND_NOT_FOUND;
    }
    if (!_handlers[command](ss, get_output())) {
      return BotStatus::FAILED;
    }
    return BotStatus::OK;
  }

  void register_handler(std::string_view command, HandlerType & h) {
    _handlers[std::string(command)] = h; //TODO: Better insert plz.
  }

  virtual BotOutputProtocol & get_output();
 private:
  std::map<std::string, HandlerType> _handlers;
};


