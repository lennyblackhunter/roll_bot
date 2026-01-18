#include "bot_setup.hh"

#include "commands.hh"

void apply_cthulhu_handlers(Bot & bot, CharacterSheetRepo & repo, std::mutex & repo_mutex) {
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
