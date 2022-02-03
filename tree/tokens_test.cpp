#include<string>
#include<iostream>

#include "tokens.hh"
#include "../sort.hh"

int main() {
    std::string s = "1 + 2 *3+(5 - 1)";
    print_vec(*tokenize(s));
    print_vec(*tokenize("123-321"));
    std::cout << tokenize("123abc").has_value() << std::endl;
}