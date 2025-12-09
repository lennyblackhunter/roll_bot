#include <ostream>
#include <set>
#include <optional>
#include <iostream>

#include "core/bot_io.hh"
#include "commands.hh"
#include "config.hh"


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
  LocalUser(const std::string & nickname) : _nickname(nickname) {}
  auto operator=(const std::string & nickname) {_nickname = nickname;};
  std::string _nickname;
  std::string get_nickname() const {
    return _nickname;
  }
  std::string get_mention() const {
    return "@" + _nickname;
  }
};


class ConsoleBot : public Bot {
  StreamBotOutput _out{&std::cout};
  std::set<std::string> _users;

  LocalUser _current_user;
  bool _shutdown = false;
 public:
  ConsoleBot(const std::set<std::string> & users): _users(users), _current_user{*users.begin()} {
    
  };

  StreamBotOutput & get_output() {
    return _out;
  }

  void run() {
    std::string line;
    while (std::cin && !_shutdown) {
      std::cout << "[" << _current_user.get_nickname() << "]: ";
      std::getline(std::cin, line);
      handle_msg(line, _current_user);
    }
    return;
  }

  void shutdown() { _shutdown = true; }

  std::optional<std::string> set_local_user(const std::string & username) {
    auto user = _users.find(username);
    if (user == _users.end()) return std::nullopt;
    _current_user = *user;
    return *user;
  }

  void add_magic_commands() {
    // Register commands that are meant for configuring bot in runtime (% commands)
    register_handler(
        "%user",
        "Show current user.",
        [this](std::istringstream & ss, const User & user, BotOutputProtocol & out){
      std::string username;
      ss >> username;
      if (!ss) return false;
      auto new_user = this->set_local_user(username);
      if (!new_user) {
        out.write_message(std::format("User {} not found.", username));
        return false;
      }
      out.write_message(std::format("Successfully inpersonating {}", *new_user));
      return true;
    });
    register_handler(
        "%exit",
        "Quit current session.",
        [this](std::istringstream & ss, const User & user, BotOutputProtocol & out){
      this->shutdown();
      return true;
    });
    register_handler(
      "%help",
      "Show this help.",
      [this](std::istringstream & ss, const User & user, BotOutputProtocol & out){
        for (const auto & [name, handler] : _handlers) {
          out.write_message(std::format("{:<20}{}", name, handler.help));
        }
        return true;
      }
    );
    register_handler(
      "%users",
      "Show list of current users.",
      [this](std::istringstream & ss, const User & user, BotOutputProtocol & out){
        auto next_user = _users.begin();
        if (next_user == _users.end()) return true;
        std::string result;
        result += *(next_user++);
        while(next_user != _users.end()) result += (" " + *(next_user++));
        out.write_message(result);
        return true;
      }
    );
  }

  LocalUser get_current_user() const {
    return _current_user;
  }
};

int main() {
  json configdocument = get_config();
  CharacterSheetRepo repo(configdocument["path_to_repo"]);
  repo.load();
  std::mutex repo_mutex;
  
  ConsoleBot bot(repo.players());
  bot.add_magic_commands();

  bot.register_handler(
      "!stat",
      "!stat [name] [stat] [dice_expr]: Sets given stat using dice_expr.",
      SetStat(repo, repo_mutex)
  );
  bot.register_handler(
      "!roll",
      "!roll [stat_prefix] [dice_expr]: Rolls for a given stat applying modifiers.", // TODO: Explain modifiers
      Roll(repo, repo_mutex)
  );
  bot.register_handler(
      "!sheet",
      "Prints current player's sheet.",
      SheetRequest(repo, repo_mutex)
  );
  bot.run();
}

