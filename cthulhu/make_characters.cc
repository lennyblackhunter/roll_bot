#include "config.hh"
#include "cthulhu/character_sheet.hh"
#include <dpp/nlohmann/json.hpp>


using nlohmann::json;

int main(int argc, char const *argv[]) {
    json configdocument = get_config();
    CharacterSheetRepo repo{configdocument["path_to_repo"]};
    repo.add(CharacterSheet{
        "JanczarKnurek",
        StatsT{
            {"wytrzymałość", Stat(11, false, StatType::IDK)},
            {"magia", Stat(10, false, StatType::IDK)},
            {"szczęście", Stat(60, false, StatType::RESOURCE)},
            {"poczytalność", Stat(50, false, StatType::RESOURCE)},
            {"siła", Stat(40, false, StatType::ATTRIBUTE)},
            {"kondycja", Stat(60, false, StatType::ATTRIBUTE)},
            {"budowa_ciała", Stat(50, false, StatType::ATTRIBUTE)},
            {"zręczność", Stat(80, false, StatType::ATTRIBUTE)},
            {"wygląd", Stat(60, false, StatType::ATTRIBUTE)},
            {"inteligencja", Stat(70, false, StatType::ATTRIBUTE)},
            {"moc", Stat(50, false, StatType::ATTRIBUTE)},
            {"wykształcenie", Stat(50, false, StatType::ATTRIBUTE)},
            {"język_ojczysty", Stat(50, false)},
            {"unik", Stat(40, false)},
            {"antropologia", Stat(61, false)},
            {"archeologia", Stat(51, false)},
            {"broń_palna(karabin/strzelba)", Stat(25, false)},
            {"broń_palna(krótka)", Stat(20, false)},
            {"charakteryzacja", Stat(5, false)},
            {"elektryka", Stat(10, false)},
            {"gadanina", Stat(5, false)},
            {"historia", Stat(75, false)},
            {"jeździectwo", Stat(5, false)},
            {"język_obcy", Stat(30, false)},
            {"księgowość", Stat(5, false)},
            {"majętność", Stat(40, false)},
            {"mechanika", Stat(10, false)},
            {"medycyna", Stat(21, false)},
            {"mity_cthulhu", Stat(0, false)},
            {"nasłuchiwanie", Stat(40, false)},
            {"nauka", Stat(51, false)},
            {"nawigacja", Stat(10, false)},
            {"obsługa_ciężkiego_sprzętu", Stat(1, false)},
            {"okultyzm", Stat(55, false)},
            {"perswazja", Stat(10, false)},
            {"pierwsza_pomoc", Stat(60, false)},
            {"pilotowanie", Stat(1, false)},
            {"pływanie", Stat(40, false)},
            {"prawo", Stat(25, false)},
            {"prowadzenie_samochodu", Stat(20, false)},
            {"psychoanaliza", Stat(1, false)},
            {"rzucanie", Stat(20, false)},
            {"skakanie", Stat(20, false)},
            {"psychologia", Stat(90, false)},
            {"spostrzegawczość", Stat(65, false)},
            {"sztuka/rzemiosło", Stat(5, false)},
            {"sztuka_przetrwania", Stat(10, false)},
            {"ślusarstwo", Stat(1, false)},
            {"tropienie", Stat(10, false)},
            {"ukrywanie", Stat(20, false)},
            {"urok_osobisty", Stat(15, false)},
            {"walka_wręcz(bijatyka)", Stat(25, false)},
            {"korzystanie_z_bibliotek", Stat(80, false)},
            {"wiedza_o_naturze", Stat(10, false)},
            {"wspinaczka", Stat(20, false)},
            {"wycena", Stat(5, false)},
            {"zastraszanie", Stat(15, false)},
            {"zręczne_palce", Stat(80, false)},
        }
    });
    repo.save();
}
