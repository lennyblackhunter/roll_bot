#include <sstream>
#include <fstream>
#include <vector>
#include <optional>
#include <stdexcept>
#include <fmt/core.h>

#include "character_sheet.hh"
#include "../calculator.hh"
#include "utils.hh"

Stat::Stat(int value, bool used) : value(value), used(used) {}

int Stat::get(Hardness h) {return value / static_cast<int>(h);}

extern const std::map<std::string, Hardness> hardness_map {
        {"normal", Hardness::NORMAL},
        {"hard", Hardness::HARD},
        {"brutal", Hardness::BRUTAL},
};

CharacterSheet::CharacterSheet(std::string name, StatsT stats): name(std::move(name)), stats(std::move(stats)) {}

RollResult CharacterSheet::roll(const StatRollRequest & request) {
    Stat & character_stat = stats[request.stat];
    int tens_dice_number = 1 + std::abs(request.modifier);
    std::vector<int> possible_tens_values(tens_dice_number);
    int tens = 0;
    int units = random_number(1, 9);
    for (int i = 0; i <= tens_dice_number; i++) {
        possible_tens_values[i] = random_number(1, 9);
    }
    if (request.modifier >= 0) {
        tens = *std::max_element(possible_tens_values.begin(), possible_tens_values.end());
    }
    else {
        tens = *std::min_element(possible_tens_values.begin(), possible_tens_values.end());
    }
    int roll_value = tens * 10 + units;
    character_stat.used = true;
    return {request.stat, roll_value, character_stat.get(request.hardness)};
}

std::string CharacterSheet::get_name() {
    return name;
}

CharacterSheetRepo::CharacterSheetRepo(std::string folder): data_folder(std::move(folder)) {
    if (not fs::is_directory(data_folder)) {
        throw RepoError(fmt::format("{} should be a path to an existing directory!", data_folder.string())); 
    }
}

void CharacterSheetRepo::add(CharacterSheet character_sheet) {
    character_sheets[character_sheet.get_name()] = character_sheet;
}

void CharacterSheetRepo::load() {
    for (const auto & entry : fs::recursive_directory_iterator(data_folder)) {
        if (entry.path().extension() != ".json") 
            continue;

        std::fstream file(entry.path(), file.in);
        if (!file.is_open())
            continue;
        json j;
        CharacterSheet character_sheet;
        file >> j;
        std::cerr << entry.path() << ": " << j << std::endl;
        character_sheet = j;
        character_sheets[character_sheet.get_name()] = character_sheet;
    }
}

void CharacterSheetRepo::save() {
    for (const auto & [name, character_sheet] : character_sheets) {
        json j = character_sheet;
        auto path_to_save = data_folder / name;
        path_to_save += ".json";
        std::fstream file(path_to_save, file.out);
        file << j;
    }
}

CharacterSheet* CharacterSheetRepo::get_character_sheet(const std::string & s) {
    if (character_sheets.count(s)) {
        return &character_sheets[s];
    }
    return nullptr;
}

template<>
std::optional<Hardness> from_str<Hardness>(const std::string_view & s) {
    auto lower_s = to_lower(s);
    try {
        return hardness_map.at(lower_s);
    } catch(std::out_of_range &) {
        return std::nullopt;
    }
}
