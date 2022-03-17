#include <string>
#include <iostream>

#include "character_sheet.hh"

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
    std::cerr << "stat: " << stat << std::endl;
    ss >> s1;
    std::cerr << s1;
    ss >> s2;
    std::cerr << s2;
    if (!consume_token(s1, modifier, hardness) || !consume_token(s2, modifier, hardness)) {
        return std::nullopt;
    }
    return StatRollRequest(stat, hardness, modifier);
}
