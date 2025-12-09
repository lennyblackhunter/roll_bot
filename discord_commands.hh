#pragma once

#include <atomic>
#include <sstream>
#include <dpp/dpp.h>

#include "cthulhu/character_sheet.hh"


void on_set_stat(std::atomic<CharacterSheetRepo*> & repo, std::stringstream & ss, const dpp::message_create_t & event, dpp::cluster & bot);

void on_roll(std::atomic<CharacterSheetRepo*> & repo, std::stringstream & ss, const dpp::message_create_t &event, dpp::cluster &bot);

void on_sheet_request(std::atomic<CharacterSheetRepo*> & repo, std::stringstream & ss, const dpp::message_create_t & event, dpp::cluster & bot);

void on_turn_off(volatile bool* button, std::stringstream & ss, const dpp::message_create_t &event, dpp::cluster &bot);
