#include <vector>
#include <ranges>
#include <sstream>
#include <fstream>
#include <optional>
#include <stdexcept>
#include <format>

#include "calculator.hh"
#include "cthulhu/character_sheet.hh"
#include "utils.hh"

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

bool RollResult::bad() const{
    return (!error.empty());
}

RollResult RollResult::bad_roll_result(std::string message) {
    return RollResult(message);
}

RollResult CharacterSheet::roll(const StatRollRequest & request) {
    std::vector<std::string> possible_names = matching_stats(request.stat);
    if (!possible_names.size()) {
        return RollResult::bad_roll_result("there is no statistic with such prefix");
    }
    else if (possible_names.size() > 1) {
        std::string stats = possible_names[0];
        for (int i = 1; i < possible_names.size(); i++) {
            stats += ", " + possible_names[i];
        }
        return RollResult::bad_roll_result(std::format(
            "choose among statistic: {}", stats));
    }
    Stat & character_stat = stats[possible_names[0]];
    int tens_dice_number = 1 + std::abs(request.modifier);
    std::vector<int> possible_tens_values(tens_dice_number);
    int tens = 0;
    int units = random_number(1, 9);
    for (auto & dice : possible_tens_values) {
        dice = random_number(1, 9);
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

std::vector<std::string> CharacterSheet::matching_stats(const std::string & prefix) const {
    std::vector<std::string> matches;
    for (const auto & stat_name : std::views::keys(stats)) {
        if (stat_name.compare(0, prefix.size(), prefix) == 0) {
            matches.push_back(stat_name);
        }
    }
    return matches;
}

std::string CharacterSheet::get_name() {
    return name;
}

void CharacterSheet::set_stat(const std::string & stat_name, int new_value) {
    auto it = stats.find(stat_name);
    if (it == stats.end()) {
        return;
    }
    it->second.value = new_value;
}

int CharacterSheet::get_stat_value(const std::string & stat_name) {
    auto it = stats.find(stat_name);
    if (it == stats.end()) {
        return 0;
    }
    return it->second.value;
}

std::ostream & operator<<(std::ostream & out, const CharacterSheet & character_sheet) {
    auto print_group = [&](const std::string & title,
                           auto predicate,
                           std::size_t columns) {
        std::vector<std::pair<std::string, int>> items;
        std::size_t name_width = 0;
        for (const auto & [stat_name, stat] : character_sheet.stats) {
            if (predicate(stat)) {
                items.emplace_back(stat_name, stat.value);
                name_width = std::max(name_width, stat_name.size());
            }
        }
        if (items.empty()) {
            return;
        }
        out << title << "\n";
        std::size_t col = 0;
        for (const auto & [stat_name, value] : items) {
            out << std::format("{:<{}} {:>3}", stat_name, name_width, value);
            col++;
            if (col == columns) {
                out << "\n";
                col = 0;
            } else {
                out << "  ";
            }
        }
        if (col != 0) {
            out << "\n";
        }
    };

    out << "```text\n";
    out << character_sheet.name << "\n";
    out << "Resources: ";
    bool first = true;
    for (const auto & [stat_name, stat] : character_sheet.stats) {
        if (stat.stat_type == StatType::IDK || stat.stat_type == StatType::RESOURCE) {
            if (!first) {
                out << " | ";
            }
            out << stat_name << " " << stat.value;
            first = false;
        }
    }
    out << "\n";
    print_group("Attributes:", [](const Stat & s){ return s.stat_type == StatType::ATTRIBUTE; }, 2);
    print_group("Abilities:", [](const Stat & s){ return s.stat_type == StatType::ABILITY; }, 3);
    out << "```\n";
    return out;
}

CharacterSheetRepo::CharacterSheetRepo(std::string folder): data_folder(std::move(folder)) {
    if (not fs::is_directory(data_folder)) {
        throw RepoError(std::format("{} should be a path to an existing directory!", data_folder.string())); 
    }
}

void CharacterSheetRepo::add(CharacterSheet character_sheet) {
    character_sheets[character_sheet.get_name()] = character_sheet;
    _players.insert(character_sheet.get_name());
}

void CharacterSheetRepo::load() {
    for (const auto & entry : fs::recursive_directory_iterator(data_folder)) {
      std::cerr << "Loading from " << entry << std::endl;
        if (entry.path().extension() != ".json") 
            continue;

        std::fstream file(entry.path(), file.in);
        if (!file.is_open())
            continue;
        json j;
        CharacterSheet character_sheet;
        file >> j;
        character_sheet = j;
        add(character_sheet);
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

const std::set<std::string> & CharacterSheetRepo::players() const {
  return _players;
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
