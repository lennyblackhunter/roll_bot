#include<string>
#include<iostream>

#include "tokens.hh"
#include "calculator.hh"

int main() {
    BinaryOperator<int> addition('+', [](int x, int y){return x + y;}, 1);
    BinaryOperator<int> subtraction('-', [](int x, int y){return x - y;}, 1);
    BinaryOperator<int> dice_roll('k', [](int x, int y){return roll_and_add(x, y);}, 3);
    std::vector<BinaryOperator<int>> operators {addition, subtraction, dice_roll};
    NodeBuilder<int> nodes(operators);
    std::string x = "69k1";
    auto node = nodes.string_to_node(x);
    std::cout << node->evaluate() << std::endl;
//    std::string s = "1 + 2 *3+(5 - 1)";
//    std::vector<char> ops {'+', '*', '-'};
//    auto token = tokenize("123abc", ops);
//    print_vec(*tokenize(s, ops));
//    print_vec(*tokenize("123-321", ops));
//    std::cout << tokenize("123abc", ops).has_value() << std::endl;
}