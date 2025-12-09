#include <string>
#include <iostream>

#include "cthulhu/character_sheet.hh"
#include "cthulhu/user_input.hh"

/*
 * TO DO:
 * validate character name and replace spaces with '_'
 * validate dice expression
 */

std::optional<StatChangeRequest> stat_change_from_string(std::istringstream & ss) {
    std::string character_name;
    std::string stat;
    ChangeType type;
    std::string s;
    ss >> character_name;
    ss >> stat;
    getline(ss, s);
    ltrim(s);
    if (s[0] == '+') {
        type = ChangeType::HIGHER;
    }
    else if (s[0] == '-') {
        type = ChangeType::LOWER;
    }
    else if (s[0] == '=') {
        type = ChangeType::SET;
    }
    else {
        return std::nullopt;
    }
    s.erase(0, 1);
    ltrim(s);
    return StatChangeRequest(character_name, stat, type, s);
}

bool consume_token(const std::string & s, int & modifier, Hardness & hardness) {
    if (s.empty()) {
        return true;
    }
    if (s[0] == '+' || s[0] == '-') {
        auto maybe_modifier = from_str<int>(s);
        if (maybe_modifier) {
            modifier = *maybe_modifier;
        }
        else {
            return false;
        }
    }
    else {
        auto maybe_hardness = from_str<Hardness>(s);
        if (maybe_hardness) {
            hardness = *maybe_hardness;
        }
        else {
            return false;
        }
    }
    return true;
}

std::optional<StatRollRequest> request_from_string(std::istream & ss) {
    std::string stat;
    Hardness hardness = Hardness::NORMAL;
    int modifier = 0;
    std::string s1;
    std::string s2;
    ss >> stat;
    ss >> s1;
    ss >> s2;
    if (!consume_token(s1, modifier, hardness) || !consume_token(s2, modifier, hardness)) {
        return std::nullopt;
    }
    return StatRollRequest(stat, hardness, modifier);
}
