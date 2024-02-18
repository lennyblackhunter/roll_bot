#include <iostream>

#include "cthulhu/character_sheet.hh"
#include "cthulhu/utils.hh"

int main() {
//    Stat s{123, false};
//    json js = s;
//    std::cout << join("+", "123", "321", "696");
    CharacterSheetRepo repo{"/home/benhauer-adm/Nie_praca/roll_bot/roll_bot/tree/cthulhu/test/repo"};
//    CharacterSheetRepo repo2{"/dupa/dupa"};

    std::string name = "Annabelle";
    std::map<std::string, Stat> stats = {
            {"wytrzymałość", Stat(12, false, StatType::IDK)},
            {"magia", Stat(7, false, StatType::IDK)},
            {"szczęście", Stat(50, false, StatType::RESOURCE)},
            {"poczytalność", Stat(50, false, StatType::RESOURCE)},
            {"siła", Stat(50, false, StatType::ATTRIBUTE)},
            {"korzystanie_z_bibliotek", Stat(30, false)},
            {"okultyzm", Stat(25, false)},
            {"urok_osobisty", Stat(30, false)},
    };
    std::string anna_name = "Anna";
    std::map<std::string, Stat> anna_stats = {
            {"wytrzymałość", Stat(12, false, StatType::IDK)},
            {"magia", Stat(7, false, StatType::IDK)},
            {"szczęście", Stat(50, false, StatType::RESOURCE)},
            {"poczytalność", Stat(50, false, StatType::RESOURCE)},
            {"siła", Stat(50, false, StatType::ATTRIBUTE)},
            {"siła", Stat(50, false, StatType::ATTRIBUTE)},
            {"siła", Stat(50, false, StatType::ATTRIBUTE)},
            {"siła", Stat(50, false, StatType::ATTRIBUTE)},
            {"siła", Stat(50, false, StatType::ATTRIBUTE)},
            {"siła", Stat(50, false, StatType::ATTRIBUTE)},
            {"korzystanie_z_bibliotek", Stat(30, false)},
            {"okultyzm", Stat(25, false)},
            {"urok_osobisty", Stat(30, false)},
    };
    CharacterSheet example_sheet = CharacterSheet(anna_name, anna_stats);
    json sheet_json = example_sheet;
    std::cout << sheet_json << std::endl;
    std::cout << "miau?" << std::endl;
    repo.load();
    std::cout << "miau" << std::endl;
    std::cout << repo.get("Stefan")->get_name() << std::endl;
    std::cout << "miau!" << std::endl;
    repo.add(example_sheet);
    std::cout << "miau!!" << std::endl;
    repo.save();
}
