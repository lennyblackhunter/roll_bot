#pragma once

#include <string>
#include <optional>

#include "character_sheet.hh"

enum class ChangeType {
    SET,
    LOWER,
    HIGHER,
};

struct StatChangeRequest {
    std::string character_name;
    std::string stat;
    ChangeType type;
    std::string dice_expression;
    StatChangeRequest(std::string character_name, std::string stat, ChangeType type, std::string dice_expression):
    character_name(std::move(character_name)), stat(std::move(stat)), type(type), dice_expression(std::move(dice_expression)) {}
};

std::optional<StatChangeRequest> stat_change_from_string(std::stringstream & ss);

bool consume_token(const std::string & s, int & modifier, Hardness & hardness);

std::optional<StatRollRequest> request_from_string(std::istream & ss);
