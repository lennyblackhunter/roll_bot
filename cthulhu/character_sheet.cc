#include <sstream>
#include <fstream>
#include <vector>
#include <optional>
#include <stdexcept>
#include <dpp/fmt/core.h>

#include "calculator.hh"
#include "cthulhu/character_sheet.hh"
#include "cthulhu/utils.hh"

Stat::Stat(int value, bool used, StatType stat_type) : value(value), used(used), stat_type(stat_type) {}

int Stat::get(Hardness h) {return value / static_cast<int>(h);}

extern const std::map<std::string, Hardness> hardness_map {
        {"normal", Hardness::NORMAL},
        {"hard", Hardness::HARD},
        {"brutal", Hardness::BRUTAL},
};

extern const std::map<StatType, std::string> stat_types_map {
        {StatType::ABILITY, "ability"},
        {StatType::ATTRIBUTE, "attribute"},
        {StatType::RESOURCE, "resource"},
        {StatType::IDK, "magic_or_health"},
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
        tens = *std::min_element(possible_tens_values.begin(), possible_tens_values.end());
    }
    else {
        tens = *std::max_element(possible_tens_values.begin(), possible_tens_values.end());
    }
    int roll_value = tens * 10 + units;
    character_stat.used = true;
    //std::cerr << "used in roll: " << character_stat.used << std::endl;
    return {request.stat, roll_value, character_stat.get(request.hardness)};
}

std::string CharacterSheet::get_name() {
    return name;
}

void CharacterSheet::set_stat(const std::string & stat_name, int new_value) {
    stats[stat_name].value = new_value;
}

int CharacterSheet::get_stat_value(const std::string & stat_name) {
    return stats[stat_name].value;
}

std::ostream & operator<<(std::ostream & out, const CharacterSheet & character_sheet) {
    out << character_sheet.name;
    for (const auto & [stat_name, stat] : character_sheet.stats) {
        if (stat.stat_type == StatType::IDK || stat.stat_type == StatType::RESOURCE) {
            out << "\t\t" << stat_name << ": " << stat.value;
        }
    }
    out << "\n\n";
    out << "Attributes:\n";
    for (const auto & [stat_name, stat] : character_sheet.stats) {
        if (stat.stat_type == StatType::ATTRIBUTE) {
            out << stat_name << ": " << stat.value << "\n";
        }
    }
    out << "\n";
    out << "Abilities:\n";
    for (const auto & [stat_name, stat] : character_sheet.stats) {
        if (stat.stat_type == StatType::ABILITY) {
            out << stat_name << ": " << stat.value << "\n";
        }
    }
    return out;
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
        character_sheet = j;
        character_sheets[character_sheet.get_name()] = character_sheet;
    }
}

void CharacterSheetRepo::save() {
    for (const auto & [name, character_sheet] : character_sheets) {
        json j = character_sheet;
        auto path_to_save = data_folder / name;
        path_to_save += ".json";
        std::cerr << "path: " << path_to_save << std::endl;
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
