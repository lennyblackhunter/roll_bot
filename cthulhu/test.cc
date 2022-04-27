#include <iostream>
#include <dpp/nlohmann/json.hpp>

#include "config.hh"
#include "cthulhu/character_sheet.hh"
#include "cthulhu/utils.hh"

using json = nlohmann::json;

int main() {
    json configdocument = get_config();

    CharacterSheetRepo repo{configdocument["path_to_repo"]};


    std::string name = "Temmie";
    std::map<std::string, Stat> stats = {
        {"wytrzymałość", Stat(13, false, StatType::IDK)},
        {"magia", Stat(0, false, StatType::IDK)},
        {"szczęście", Stat(28, false, StatType::RESOURCE)},
        {"poczytalność", Stat(44, false, StatType::RESOURCE)},
        {"siła", Stat(50, false, StatType::ATTRIBUTE)},
        {"kondycja", Stat(75, false, StatType::ATTRIBUTE)},
        {"budowa_ciała", Stat(55, false, StatType::ATTRIBUTE)},
        {"zręczność", Stat(60, false, StatType::ATTRIBUTE)},
        {"wygląd", Stat(65, false, StatType::ATTRIBUTE)},
        {"inteligencja", Stat(60, false, StatType::ATTRIBUTE)},
        {"moc", Stat(50, false, StatType::ATTRIBUTE)},
        {"wykształcenie", Stat(62, false, StatType::ATTRIBUTE)},
        {"język_ojczysty", Stat(62, false)},
        {"unik", Stat(30, false)},
        {"antropologia", Stat(1, false)},
        {"archeologia", Stat(1, false)},
        {"broń_palna(karabin/strzelba)", Stat(25, false)},
        {"broń_palna(krótka)", Stat(20, false)},
        {"charakteryzacja", Stat(5, false)},
        {"elektryka", Stat(10, false)},
        {"gadanina", Stat(5, false)},
        {"historia", Stat(5, false)},
        {"jeździectwo", Stat(5, false)},
        {"angielski", Stat(33, false)},
        {"księgowość", Stat(5, false)},
        {"majętność", Stat(0, false)},
        {"mechanika", Stat(10, false)},
        {"medycyna", Stat(1, false)},
        {"mity_cthulhu", Stat(0, false)},
        {"nasłuchiwanie", Stat(20, false)},
        {"nawigacja", Stat(10, false)},
        {"obsługa_ciężkiego_sprzętu", Stat(1, false)},
        {"okultyzm", Stat(5, false)},
        {"perswazja", Stat(80, false)},
        {"pierwsza_pomoc", Stat(30, false)},
        {"pilotowanie", Stat(1, false)},
        {"pływanie", Stat(20, false)},
        {"prawo", Stat(5, false)},
        {"prowadzenie_samochodu", Stat(20, false)},
        {"psychoanaliza", Stat(1, false)},
        {"rzucanie", Stat(20, false)},
        {"skakanie", Stat(20, false)},
        {"psychologia", Stat(56, false)},
        {"spostrzegawczość", Stat(58, false)},
        {"sztuka/rzemiosło", Stat(5, false)},
        {"sztuka_przetrwania", Stat(13, false)},
        {"ślusarstwo", Stat(1, false)},
        {"tropienie", Stat(10, false)},
        {"ukrywanie", Stat(20, false)},
        {"urok_osobisty", Stat(15, false)},
        {"walka_wręcz(bijatyka)", Stat(25, false)},
        {"wiedza_o_naturze", Stat(10, false)},
        {"wspinaczka", Stat(20, false)},
        {"wycena", Stat(75, false)},
        {"zastraszanie", Stat(15, false)},
        {"zręczne_palce", Stat(10, false)},
    };

    CharacterSheet example_sheet = CharacterSheet(name, stats);
    json sheet_json = example_sheet;
    std::cout << sheet_json << std::endl;
    std::cout << "miau?" << std::endl;
    repo.load();
    std::cout << "miau" << std::endl;
    std::cout << "miau!" << std::endl;
    repo.add(example_sheet);
    std::cout << "miau!!" << std::endl;
    repo.save();
}
