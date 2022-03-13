#include <iostream>

#include "character_sheet.hh"
#include "utils.hh"

int main() {
//    Stat s{123, false};
//    json js = s;
//    std::cout << join("+", "123", "321", "696");
    CharacterSheetRepo repo{"/home/benhauer-adm/Nie_praca/roll_bot/roll_bot/tree/cthulhu/test/repo"};
//    CharacterSheetRepo repo2{"/dupa/dupa"};

    std::string name = "Annabelle";
    std::map<std::string, Stat> stats = {
            {"korzystanie_z_bibliotek", Stat(30, false)},
            {"okultyzm", Stat(25, false)},
            {"urok_osobisty", Stat(30, false)},
    };
    CharacterSheet example_sheet = CharacterSheet(name, stats);
    json sheet_json = example_sheet;
//    std::cout << sheet_json << std::endl;
    repo.load();
    std::cout << repo.get_character_sheets()["Stefan"].get_name() << std::endl;
    repo.add(example_sheet);
    repo.save();
}
