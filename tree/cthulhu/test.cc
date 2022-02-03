#include <iostream>

#include "character_sheet.hh"
#include "utils.hh"

int main() {
    Stat s{123, false};
    json js = s;
    std::cout << join("+", "123", "321", "696");
    CharacterSheetRepo repo{"/home/janczarknurek"};
    CharacterSheetRepo repo2{"/dupa/dupa"};
}
