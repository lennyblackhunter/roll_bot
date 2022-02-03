#pragma once

#include <vector>
#include <optional>

#include "tokens.hh"

std::optional<std::vector<Token>> to_rpn(const std::vector<Token> & infix_notation);