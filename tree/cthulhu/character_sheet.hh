#include <string>
#include <map>
#include <filesystem>

#include <nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

enum Hardness {
    NORMAL = 1,
    HARD = 2,
    BRUTAL = 5
};

struct Stat {
    int value;
    bool used;
    Stat(int value, bool used=false);
    Stat() = default;
    int get(Hardness hardness = NORMAL);
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Stat, value, used);


using StatsT = std::map<std::string, Stat>;

struct StatRollRequest {
    std::string stat;
    Hardness hardness;
    int modifier;
    StatRollRequest() = default;
    StatRollRequest(std::string stat, Hardness hardness, int modifier): stat(std::move(stat)), hardness(hardness), modifier(modifier) {}
};

class CharacterSheet {
    std::string name;
    StatsT stats;
public:

    CharacterSheet() = default;
    CharacterSheet(std::string name, StatsT stats);

    int roll(const StatRollRequest & r);

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CharacterSheet, name, stats);
};

struct RepoError : public std::invalid_argument {
    using std::invalid_argument::invalid_argument;
};

class CharacterSheetRepo {
    fs::path data_folder;
public:

    CharacterSheetRepo(std::string folder);

    void load();
};
