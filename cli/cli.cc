#include <ostream>
#include <iostream>

#include "core/bot_io.hh"


class StreamBotOutput : public BotOutputProtocol {
  std::ostream * _out;

 public:
  explicit StreamBotOutput(std::ostream * out): _out(out) {}

  void write_message(std::string_view msg) {
    *_out << msg << std::endl;
  }
};


class LocalUser: public User {
 public:
  std::string _nickname;
  std::string get_nickname() {
    return _nickname;
  }
  std::string get_mention() {
    return "@" + _nickname;
  }
};


class ConsoleBot : public Bot {
  StreamBotOutput _out{&std::cout};
  LocalUser _current_user{""}
 public:
  StreamBotOutput & get_output() {
    return _out;
  }

  void run() {
    std::string line;
    while (std::cin) {
      std::getline(std::cin, line);
      handle_msg(line);
    }
    return;
  }
};

int main() {

}
