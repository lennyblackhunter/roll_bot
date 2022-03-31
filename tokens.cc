#include <vector>
#include <string>
#include <cctype>
#include <map>
#include <optional>
#include <iostream>

#include "tokens.hh"


Token::Token(TokenType token_type, std::string str) : token_type(token_type), value(std::move(str)) {};


std::ostream & operator<<(std::ostream & out, const Token & token) {
    return out << token.token_type << " " << token.value;
}

bool Token::operator==(const Token & token) const {
    return token.token_type == token_type && token.value == value;
}

std::optional<std::vector<Token> > tokenize(const std::string & str, const std::vector<char> & ops) {
    std::map<char, TokenType> token_map {
        {'(', LEFT_BRACKET},
        {')', RIGHT_BRACKET},
    };
    for (char op: ops) {
        token_map[op] = OPERATOR;
    }
    std::vector<Token> v;
    std::string substr;
    for (char character : str) {
        if (isdigit(character)) {
            substr += character;
        }
        else {
            if (!substr.empty()) {
                v.emplace_back(TOKEN_LITERAL, substr);
                substr = "";
            }
            if (isspace(character)) {
                continue;
            }
            if (!token_map.count(character)) {
                return std::nullopt;
            }
            v.emplace_back(token_map[character], std::string{character});
        }
    }
    if (!substr.empty()) {
        v.emplace_back(TOKEN_LITERAL, substr);
        substr = "";
    }
    return v;
}
