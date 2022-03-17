#include <map>
#include <string>
#include <sstream>
#include <dpp/dpp.h>
#include <fmt/core.h>

#include "cthulhu/user_input.hh"
#include "cthulhu/character_sheet.hh"

void on_roll(CharacterSheetRepo & repo, std::stringstream & ss, const dpp::message_create_t & event, dpp::cluster & bot) {
    std::string answer;
    auto request = request_from_string(ss);
    if (request) {
        std::string character_name = event.msg.member.nickname;
        std::cerr << "data folder: " << repo.data_folder << std::endl;
        auto character_sheet = repo.get_character_sheet(character_name);
        if (character_sheet)  {
            if (character_sheet->stats.count(request->stat)) {
//                std::cerr << "name: " << character_name << std::endl;
//                std::cerr << "used: " << character_sheet->stats[request->stat].used << std::endl;
//                std::cerr << "value: " << character_sheet->stats[request->stat].value << std::endl;
                RollResult result = character_sheet->roll(*request);
                answer = fmt::format("{} rolled {} on {}, should be below {}",
                                     character_name, result.result, result.stat, result.target);
            }
            else {
                answer = "nie ma takiej statystyki";
            }

        }
        else {
            answer = "nie ma takiej postaci";
        }
    }
    else {
        answer = "to nie jest poprawny napis";
    }
    bot.message_create(
            dpp::message(
                    event.msg.channel_id,
                    answer));
}
