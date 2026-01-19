#include "bot_setup.hh"

#include <format>

#include "commands.hh"

void apply_cthulhu_handlers(Bot & bot, CharacterSheetRepo & repo, std::mutex & repo_mutex) {
  bot.register_handler(
      "!help",
      "Show available commands.",
      [&bot](std::istringstream & ss, const User & user, BotOutputProtocol & out){
        std::string help_text;
        for (const auto & entry : bot.list_handlers()) {
          help_text += std::format("{:<20}{}\n", entry.command, entry.help);
        }
        out.write_message(help_text, OutputFormat{.table = true});
        return true;
      }
  );
  bot.register_handler(
      "!stat",
      "!stat [name] [stat] [dice_expr]: Sets given stat using dice_expr.",
      SetStat(repo, repo_mutex)
  );
  bot.register_handler(
      "!roll",
      "!roll [stat_prefix] [dice_expr]: Rolls for a given stat applying modifiers.",
      Roll(repo, repo_mutex)
  );
  bot.register_handler(
      "!sheet",
      "Prints current player's sheet.",
      SheetRequest(repo, repo_mutex)
  );
}
