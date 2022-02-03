#include <sstream>
#include <fstream>

#include <fmt/core.h>

#include "character_sheet.hh"
#include "utils.hh"

Stat::Stat(int value, bool used) : value(value), used(used) {}

int Stat::get(Hardness h) {return value / h;}

CharacterSheet::CharacterSheet(std::string name, StatsT stats): name(std::move(name)), stats(std::move(stats)) {}

int CharacterSheet::roll(const StatRollRequest & r) {
    return 42;
}

CharacterSheetRepo::CharacterSheetRepo(std::string folder): data_folder(std::move(folder)) {
    if (not fs::is_directory(data_folder)) {
        throw RepoError(fmt::format("{} should be a path to an existing directory!", data_folder.string())); 
    }
}

void CharacterSheetRepo::load() {
    for (const auto & entry : fs::recursive_directory_iterator(data_folder)) {
        if (entry.path().extension() != ".json") 
            continue;

        std::fstream file(entry.path(), file.in);
        if (!file.is_open()) 
            continue;
    }
}
