#include <vector>
#include <iostream>

#include "rpn.hh"
#include "tokens.hh"


void test1() {
    std::vector<Token> input{
        Token(TokenType::TOKEN_LITERAL, "123"),
        Token(TokenType::OPERATOR, "+"),
        Token(TokenType::TOKEN_LITERAL, "321")
    };

    std::vector<Token> expected{
            Token(TokenType::TOKEN_LITERAL, "123"),
            Token(TokenType::TOKEN_LITERAL, "321"),
            Token(TokenType::OPERATOR, "+"),
    };

    if (to_rpn(input) != expected) {
        std::cerr << "Error on 1";
    }
}

void test2() {
    std::vector<Token> input{
            Token(TokenType::LEFT_BRACKET, "("),
            Token(TokenType::TOKEN_LITERAL, "123"),
            Token(TokenType::OPERATOR, "+"),
            Token(TokenType::TOKEN_LITERAL, "321"),
            Token(TokenType::RIGHT_BRACKET, ")"),
            Token(TokenType::OPERATOR, "*"),
            Token(TokenType::TOKEN_LITERAL, "111")
    };

    std::vector<Token> expected{
            Token(TokenType::TOKEN_LITERAL, "123"),
            Token(TokenType::TOKEN_LITERAL, "321"),
            Token(TokenType::OPERATOR, "+"),
            Token(TokenType::TOKEN_LITERAL, "111"),
            Token(TokenType::OPERATOR, "*"),

    };

    if (to_rpn(input) != expected) {
        std::cerr << "Error on 2";
    }
}

void test3() {
    std::vector<Token> input{
        Token(TokenType::RIGHT_BRACKET, ")"),
        Token(TokenType::TOKEN_LITERAL, "123"),
        Token(TokenType::OPERATOR, "+"),
        Token(TokenType::TOKEN_LITERAL, "321"),
        Token(TokenType::LEFT_BRACKET, "("),
        Token(TokenType::OPERATOR, "*"),
        Token(TokenType::TOKEN_LITERAL, "111")
    };

    if (to_rpn(input).has_value()) {
        std::cerr << "Error on 3!";
    }
}

int main() {
    test1();
    test2();
    test3();
    std::vector<Token> input{
        Token(TOKEN_LITERAL, "123"),
        Token(OPERATOR, "+"),
        Token(OPERATOR, "+"),
        Token(TOKEN_LITERAL, "321")
    };
}