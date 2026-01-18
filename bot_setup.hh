#pragma once

#include <mutex>

#include "core/bot_io.hh"
#include "cthulhu/character_sheet.hh"

void apply_cthulhu_handlers(Bot & bot, CharacterSheetRepo & repo, std::mutex & repo_mutex);
