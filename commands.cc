#include <string>
#include <sstream>
#include <format>

#include "core/bot_io.hh"
#include "cthulhu/user_input.hh"
#include "cthulhu/character_sheet.hh"
#include "calculator.hh"
#include "commands.hh"


CthulhuBotCommand::CthulhuBotCommand(
    CharacterSheetRepo & repo,
    std::mutex & repo_mutex
  ): repo(repo), repo_mutex(repo_mutex) {} 



static std::string join_matches(const std::vector<std::string> & matches) {
  if (matches.empty()) {
    return "";
  }
  std::string result = matches.front();
  for (std::size_t i = 1; i < matches.size(); ++i) {
    result += ", " + matches[i];
  }
  return result;
}

bool SetStat::operator()(std::istringstream & in, const User & user, BotOutputProtocol & out) {
  std::string answer;
  auto probably_request = stat_change_from_string(in);
  if (!probably_request) {
    return false;
  }
  auto & request = probably_request.value();
  BinaryOperator<int> addition('+', [](int x, int y){return x + y;}, 1);
  BinaryOperator<int> subtraction('-', [](int x, int y){return x - y;}, 1);
  BinaryOperator<int> dice_roll('k', [](int x, int y){return roll_and_add(x, y);}, 3);
  NodeBuilder<int> nodes({addition, subtraction, dice_roll});
  std::lock_guard l(repo_mutex);  // You're taking too long?
  auto character_sheet = repo.get_character_sheet(request.character_name);
  if (!character_sheet) {
    out.write_message("no such character");
    return false;
  }
  auto matches = character_sheet->matching_stats(request.stat);
  if (matches.empty()) {
    out.write_message("no such statistic");
    return false;
  }
  if (matches.size() > 1) {
    out.write_message(std::format("ambiguous statistic: {}", join_matches(matches)));
    return false;
  }
  const std::string & stat_name = matches.front();
  auto node = nodes.string_to_node(request.dice_expression);
  if (!node) {
      std::cerr << "'" << request.dice_expression << "' is not a valid dice expression.";
      return false;
  }
  int new_value;
  int roll_value = node->evaluate();
  int old_value = character_sheet->get_stat_value(stat_name);
  if (request.type == ChangeType::SET) {
      new_value = roll_value;
  }
  else if (request.type == ChangeType::HIGHER) {
      new_value = old_value + roll_value;
  }
  else {
      new_value = old_value - roll_value;
  }
  character_sheet->set_stat(stat_name, new_value);
  answer = std::format("you rolled {} - {}'s {} was changed from {} to {}",
                       roll_value, request.character_name, stat_name, old_value, new_value);
  std::cerr << "answer: " << answer << std::endl;
  out.write_message(answer);
  return true;
}


static std::string test_result(const User & user, const RollResult & result) {
    std::string answer = result.error;
    if (!result.bad()) {
        answer = std::format("{} tested {} ({}) and got {} - test passed OwO",
                            user.get_mention(), result.stat, result.target, result.result);
        if (result.result > result.target) {
            int diff = result.result - result.target;
            answer = std::format("{} tested {} ({}) and got {} - test failed, you need to use {} luck points",
                                user.get_mention(), result.stat, result.target, result.result, diff);
        }
    }
    return answer;
}

bool Roll::operator()(std::istringstream & in, const User & user, BotOutputProtocol & out) {
    std::string answer = "i don't understand :<";
    auto request = request_from_string(in);
    std::lock_guard l(repo_mutex);
    if (request) {
        std::string character_name = user.get_nickname();
        std::cerr << "Roll request from player " << character_name << std::endl;
        auto character_sheet = repo.get_character_sheet(character_name);
        answer = "no such character";
        if (character_sheet)  {
            answer = "no such statistic";
            RollResult result = character_sheet->roll(*request);
            answer = test_result(user, result);
        }
    }
    std::cerr << "answer: " << answer << std::endl;
    out.write_message(answer);
    return true;
}

bool SheetRequest::operator()(std::istringstream & in, const User & user, BotOutputProtocol & out) {
    std::string character_name;
    getline(in, character_name);
    ltrim(character_name);
    std::lock_guard l(repo_mutex);
    if (!in) {
        character_name = user.get_nickname();
    }
    auto character_sheet = repo.get_character_sheet(character_name);
    if (!character_sheet) {
        std::cerr << "Could not found character sheet of '" << character_name << "'\n";
        return false;
    }
    out.write_message(to_string(*character_sheet), OutputFormat{.table = true});
    return true;
}
