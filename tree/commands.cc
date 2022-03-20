#include <string>
#include <atomic>
#include <sstream>
#include <dpp/dpp.h>
#include <fmt/core.h>

#include <dpp/nlohmann/json.hpp>

#include "cthulhu/user_input.hh"
#include "cthulhu/character_sheet.hh"

std::string test_result(const dpp::message_create_t & event, const RollResult & result) {
    std::string answer;
    answer = fmt::format("{} tested {} ({}) and got {} - test passed OwO",
                         event.msg.member.get_mention(), result.stat, result.target, result.result);
    if (result.result > result.target) {
        int diff = result.result - result.target;
        answer = fmt::format("{} tested {} ({}) and got {} - test failed, you need to use {} luck points",
                             event.msg.member.get_mention(), result.stat, result.target, result.result, diff);
    }
    return answer;
}

void on_roll(std::atomic<CharacterSheetRepo*> & repo, std::stringstream & ss, const dpp::message_create_t & event, dpp::cluster & bot) {
    std::string answer = "i don't understand :<";
    auto request = request_from_string(ss);
    if (request) {
        std::string character_name = event.msg.member.nickname;
        auto character_sheet = repo.load()->get_character_sheet(character_name);
        answer = "no such character";
        if (character_sheet)  {
            answer = "no such stat";
            if (character_sheet->stats.count(request->stat)) {
                json j = *character_sheet;
                std::cerr << j << std::endl;
                RollResult result = character_sheet->roll(*request);
                json l = *character_sheet;
                std::cerr << l << std::endl;
                answer = test_result(event, result);
            }
        }
    }
    bot.message_create(dpp::message(event.msg.channel_id,answer));
}

void on_turn_off(volatile bool* button, std::stringstream & ss, const dpp::message_create_t &event, dpp::cluster &bot) {
    *button = false;
}
