#pragma once

#include <mutex>
#include <sstream>

#include "cthulhu/character_sheet.hh"
#include "core/bot_io.hh"


struct CthulhuBotCommand {
  CharacterSheetRepo & repo;
  std::mutex & repo_mutex;
  CthulhuBotCommand(CharacterSheetRepo & repo, std::mutex & repo_mutex);
};

#define command_handler(name) struct name: CthulhuBotCommand { \
  using CthulhuBotCommand::CthulhuBotCommand; \
  bool operator()(std::istringstream &, const User &, BotOutputProtocol &); \
};


command_handler(SetStat);
command_handler(Roll);
command_handler(SheetRequest);
