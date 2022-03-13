#pragma once

#include <string>
#include <optional>

#include "character_sheet.hh"

bool consume_token(const std::string & s, int & modifier, Hardness & hardness);

std::optional<StatRollRequest> request_from_string(std::istream & ss);
