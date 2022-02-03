#include<string>
#include<iostream>

#include "tokens.hh"
#include "../sort.hh"

int main() {
    std::string s = "1 + 2 *3+(5 - 1)";
    std::vector<char> ops {'+', '*', '-'};
    print_vec(*tokenize(s, ops));
    print_vec(*tokenize("123-321", ops));
    std::cout << tokenize("123abc", ops).has_value() << std::endl;
}