#include <vector>
#include <string>
#include <stack>
#include <map>

#include "rpn.hh"

std::optional<std::vector<Token>> to_rpn(const std::vector<Token> & infix_notation) {
    std::map<std::string, int> operator_precedence_map {
            {"k", 3},
            {"+", 1},
            {"-", 1},
            {"*", 2},
            {"/", 2}
    };
    std::map<std::string, AssociativityType> operator_associativity_map {
            {"k", RIGHT},
            {"+", RIGHT},
            {"-", LEFT},
            {"*", RIGHT},
            {"/", LEFT}
    };
    std::vector<Token> rpn;
    std::stack<Token> operator_stack;
    for (const Token & token : infix_notation) {
        if (token.token_type == TOKEN_LITERAL) {
            rpn.push_back(token);
        }
        else if (token.token_type == LEFT_BRACKET) {
            operator_stack.push(token);
        }
        else if (token.token_type == RIGHT_BRACKET) {
            while (!operator_stack.empty()) {
                Token top_token = operator_stack.top();
                operator_stack.pop();
                if (top_token.token_type == LEFT_BRACKET) {
                    break;
                }
                if (operator_stack.empty()) {
                    return std::nullopt;
                }
                rpn.push_back(top_token);
            }
        }
        else if (token.token_type == OPERATOR) {
            const auto op_prec = operator_precedence_map[token.value];
            const auto op_asoc = operator_associativity_map[token.value];

            while (not operator_stack.empty()) {
                Token top_token = operator_stack.top();
                if (op_asoc == LEFT && op_prec <= operator_precedence_map[top_token.value] ||
                    op_asoc == RIGHT && op_prec < operator_precedence_map[top_token.value]
                ) {
                    rpn.push_back(top_token);
                    operator_stack.pop();
                } else {
                    break;
                }
            }
            operator_stack.push(token);
        }
    }
    while (!operator_stack.empty()) {
        Token top_token = operator_stack.top();
        operator_stack.pop();
        if (top_token.token_type == LEFT_BRACKET) {
            return std::nullopt;
        }
        rpn.push_back(top_token);
    }
    return rpn;
}
