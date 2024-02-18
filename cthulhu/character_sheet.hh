#pragma once

#include <string>
#include <map>
#include <optional>
#include <filesystem>

#include "cthulhu/utils.hh"

#include <dpp/nlohmann/json.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

enum class Hardness {
    NORMAL = 1,
    HARD = 2,
    BRUTAL = 5,
};

enum class StatType {
    ABILITY,
    ATTRIBUTE,
    //luck and sanity
    RESOURCE,
    //magic and health
    IDK,
};

extern const std::map<std::string, Hardness> hardness_map;

template<>
std::optional<Hardness> from_str<Hardness>(const std::string_view & s);

struct Stat {
    int value;
    bool used;
    StatType stat_type;
    Stat() = default;
    explicit Stat(int value, bool used=false, StatType stat_type=StatType::ABILITY);
    int get(Hardness hardness = Hardness::NORMAL);
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Stat, value, used, stat_type);


using StatsT = std::map<std::string, Stat>;

struct StatRollRequest {
    std::string stat;
    Hardness hardness;
    int modifier;
    StatRollRequest() = default;
    StatRollRequest(std::string stat, Hardness hardness, int modifier): stat(std::move(stat)), hardness(hardness), modifier(modifier) {}
};

struct RollResult {
    std::string stat;
    std::string error;
    int result;
    int target;
    RollResult(std::string stat, int result, int target): stat(std::move(stat)), result(result), target(target) {}
    private:
    explicit RollResult(std::string error): error(error) {};
    
    public:
    bool bad() const;
    static RollResult bad_roll_result(std::string message);
};

class CharacterSheet {
    std::string name;
    //StatsT stats;
public:
    StatsT stats;

    CharacterSheet() = default;
    CharacterSheet(std::string name, StatsT stats);

    virtual RollResult roll(const StatRollRequest & r);
    std::string get_name();
    int get_stat_value(const std::string & stat_name);
    virtual void set_stat(const std::string & stat_name, int new_value);

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(CharacterSheet, name, stats);

    friend
    std::ostream & operator<<(std::ostream & out, const CharacterSheet & character_sheet);
};

struct RepoError : public std::invalid_argument {
    using std::invalid_argument::invalid_argument;
};



class BaseCharacterSheetRepo {
 public:
  virtual CharacterSheet* get(std::string_view id) = 0;
  virtual std::vector<std::string_view> ids() = 0;
  virtual void load() = 0;
  void save();
  virtual void save(std::string_view id) = 0;
};


class CharacterSheetRepo: public BaseCharacterSheetRepo {
    //fs::path data_folder;
    std::map<std::string, CharacterSheet, std::less<>> character_sheets;
public:
    fs::path data_folder;
    explicit CharacterSheetRepo(std::string folder);

    CharacterSheet* get(std::string_view id) override;
    void add(CharacterSheet character_sheet);
    std::vector<std::string_view> ids() override;
    void load() override;
    using BaseCharacterSheetRepo::save;
    void save(std::string_view id) override;
};

class AutoSaveCSWrapper: CharacterSheet {
  CharacterSheet* _wrapped;
  BaseCharacterSheetRepo& _repo;
 public:
  AutoSaveCSWrapper(CharacterSheet* wrapped, BaseCharacterSheetRepo& repo): _wrapped(wrapped), _repo(repo) {};
  RollResult roll(const StatRollRequest & r) override {
    auto result = _wrapped->roll(r);
    _repo.save();  // TODO: save just one
    return result;
  }
  void set_stat(const std::string & stat_name, int new_value) override {
    _wrapped->set_stat(stat_name, new_value);
    _repo.save();
  }
};

class AutoSaveCSRepoWrapper: public BaseCharacterSheetRepo {
  BaseCharacterSheetRepo * _wrapped;
 public:
  explicit AutoSaveCSRepoWrapper(BaseCharacterSheetRepo * wrapped): _wrapped(wrapped) {};
  CharacterSheet* get(std::string_view id) override {
    return _wrapped->get(id);
  };
  void add(CharacterSheet character_sheet);
  std::vector<std::string_view> ids() override;
  void load() override;
  using BaseCharacterSheetRepo::save;
  void save(std::string_view id) override;
};