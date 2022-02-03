#pragma once

#include <optional>
#include <vector>
#include <map>

enum TokenType {
    OPERATOR,
    TOKEN_LITERAL,
    LEFT_BRACKET,
    RIGHT_BRACKET,
};

enum AssociativityType {
    LEFT,
    RIGHT,
};

struct Token {
    TokenType token_type;
    std::string value;
public:
    Token(TokenType token_type, std::string str);
    bool operator==(const Token &) const;
};

std::ostream & operator<<(std::ostream & out, const Token & token);

std::optional<std::vector<Token> > tokenize(const std::string & str, const std::vector<char> &);
