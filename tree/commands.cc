#include <string>
#include <atomic>
#include <sstream>
#include <dpp/dpp.h>
#include <fmt/core.h>

#include <dpp/nlohmann/json.hpp>

#include "cthulhu/user_input.hh"
#include "cthulhu/character_sheet.hh"
#include "calculator.hh"

void on_set_stat(std::atomic<CharacterSheetRepo*> & repo, std::stringstream & ss, const dpp::message_create_t & event, dpp::cluster & bot) {
    std::string answer;
    auto probably_request = stat_change_from_string(ss);
    if (!probably_request) {
        return;
    }
    auto & request = probably_request.value();
    BinaryOperator<int> addition('+', [](int x, int y){return x + y;}, 1);
    BinaryOperator<int> subtraction('-', [](int x, int y){return x - y;}, 1);
    BinaryOperator<int> dice_roll('k', [](int x, int y){return roll_and_add(x, y);}, 3);
    NodeBuilder<int> nodes({addition, subtraction, dice_roll});
    auto character_sheet = repo.load()->get_character_sheet(request.character_name);
    auto node = nodes.string_to_node(request.dice_expression);
    if (!node) {
        std::cerr << "'" << request.dice_expression << "' is not a valid dice expression.";
        return;
    }
    int new_value;
    int roll_value = node->evaluate();
    int old_value = character_sheet->get_stat_value(request.stat);
    if (request.type == ChangeType::SET) {
        new_value = roll_value;
    }
    else if (request.type == ChangeType::HIGHER) {
        new_value = old_value + roll_value;
    }
    else {
        new_value = old_value - roll_value;
    }
    character_sheet->set_stat(request.stat, new_value);
    answer = fmt::format("you rolled {} - {}'s {} was changed from {} to {}",
                         roll_value, request.character_name, request.stat, old_value, new_value);
    std::cerr << "answer: " << answer << std::endl;
    bot.message_create(dpp::message(event.msg.channel_id, answer));
}


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
    bot.message_create(dpp::message(event.msg.channel_id, answer));
}

void on_turn_off(volatile bool* button, std::stringstream & ss, const dpp::message_create_t &event, dpp::cluster &bot) {
    *button = false;
}
