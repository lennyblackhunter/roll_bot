#include <iostream>
#include <mutex>

#include <dpp/nlohmann/json.hpp>

#include "bot.h"
#include "bot_setup.hh"
#include "config.hh"
#include "cthulhu/character_sheet.hh"
#include "discord/discord_bot.hh"

using json = nlohmann::json;

int main(int argc, char const *argv[]) {
    json configdocument = get_config();
    if (!configdocument.contains("path_to_repo") || !configdocument["path_to_repo"].is_string()) {
        std::cerr << "Config error: 'path_to_repo' must be a string." << std::endl;
        return 1;
    }
    if (!configdocument.contains("token") || !configdocument["token"].is_string()) {
        std::cerr << "Config error: 'token' must be a string." << std::endl;
        return 1;
    }
    CharacterSheetRepo repo(configdocument["path_to_repo"]);
    repo.load();
    std::mutex repo_mutex;

    DiscordBot bot(configdocument["token"]);
    apply_cthulhu_handlers(bot, repo, repo_mutex);
    bot.register_handler(
        "!goodbye",
        "Quit current session.",
        [&bot](std::istringstream & ss, const User & user, BotOutputProtocol & out){
          bot.request_shutdown();
          return true;
        }
    );
    bot.run();
    repo.save();
    return 0;
};
