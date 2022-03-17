#pragma once

#include <sstream>
#include <dpp/dpp.h>

#include "cthulhu/character_sheet.hh"

void on_roll(CharacterSheetRepo & repo, std::stringstream & ss, const dpp::message_create_t &event, dpp::cluster &bot);
