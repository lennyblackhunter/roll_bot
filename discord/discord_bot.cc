#include "discord/discord_bot.hh"

#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>

DiscordBot * DiscordBot::_active_bot = nullptr;

DiscordOutput::DiscordOutput(dpp::cluster * bot, dpp::snowflake channel_id)
    : _bot(bot), _channel_id(channel_id) {}

void DiscordOutput::write_message(std::string_view msg) {
  _bot->message_create(dpp::message(_channel_id, std::string(msg)));
}

DiscordUser::DiscordUser(const dpp::guild_member & member)
    : _nickname(member.get_nickname()), _mention(member.get_mention()) {}

std::string DiscordUser::get_nickname() const {
  return _nickname;
}

std::string DiscordUser::get_mention() const {
  return _mention;
}

DiscordBot::DiscordBot(std::string token)
    : _bot(std::move(token), dpp::i_default_intents | dpp::i_message_content) {}

void DiscordBot::signal_handler(int signal) {
  if (_active_bot) {
    _active_bot->request_shutdown();
  }
}

void DiscordBot::request_shutdown() {
  _shutdown = true;
}

void DiscordBot::run() {
  _active_bot = this;
  std::signal(SIGINT, signal_handler);
  std::signal(SIGTERM, signal_handler);

  _bot.on_log([this](const dpp::log_t &event) {
    if (event.severity >= dpp::ll_debug) {
      std::cout << dpp::utility::current_date_time() << " ["
                << dpp::utility::loglevel(event.severity) << "] "
                << event.message << "\n";
    }
  });

  _bot.on_message_create([this](const dpp::message_create_t &event) {
    if (event.msg.author.is_bot()) {
      return;
    }
    DiscordUser user(event.msg.member);
    DiscordOutput out(&_bot, event.msg.channel_id);
    handle_msg(event.msg.content, user, out);
  });

  _bot.start(dpp::start_type::st_return);
  while (!_shutdown) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}
