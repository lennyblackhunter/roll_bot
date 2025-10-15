#pragma once

#include <functional>
#include <stringstream>
#include <string_view>
#include <format>


class BotOutputProtocol {
  virtual void write_message(std::string_view msg) = 0;
}

class enum BotStatus {
  OK = 0;
  COMMAND_NOT_FOUND = 1;
  FAILED = 2;
};

class Bot {
 public:
  using HandlerType = std::function<bool(std::stringstream &, BotOutputProtocol &)>;

  BotStatus handle_msg(std::string_view msg) { 
    std::stringstream ss(msg);
    std::string command;
    ss >> command;
    auto handler = _handlers.find(command);
    if (!handler) {
      get_output().write_message(std::format::format("Command {} not found.", command));
      return BotStatus::COMMAND_NOT_FOUND;
    }
    if (!_handlers[command](ss, get_output())) {
      return BotStatus::FAILED;
    }
    return BotStatus::OK;
  }

  virtual BotOutputProtocol & get_output();
 private:
  std::map<std::string, HandlerType> _handlers;
};


