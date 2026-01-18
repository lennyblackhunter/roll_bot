#pragma once

#include <atomic>
#include <string>

#include <dpp/dpp.h>

#include "core/bot_io.hh"

class DiscordOutput : public BotOutputProtocol {
  dpp::cluster * _bot;
  dpp::snowflake _channel_id;

 public:
  DiscordOutput(dpp::cluster * bot, dpp::snowflake channel_id);
  void write_message(std::string_view msg) override;
};

class DiscordUser : public User {
  std::string _nickname;
  std::string _mention;

 public:
  explicit DiscordUser(const dpp::guild_member & member);
  std::string get_nickname() const override;
  std::string get_mention() const override;
};

class DiscordBot : public Bot {
 public:
  explicit DiscordBot(std::string token);
  void run();
  void request_shutdown();

 private:
  dpp::cluster _bot;
  std::atomic<bool> _shutdown{false};

  static DiscordBot * _active_bot;
  static void signal_handler(int signal);
};
