#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>

#include "cthulhu/character_sheet.hh"

namespace fs = std::filesystem;

int main() {
    int failures = 0;
    auto expect = [&](bool condition, const std::string & message) {
        if (!condition) {
            std::cerr << "FAIL: " << message << "\n";
            failures++;
        }
    };

    const char * runfiles_root = std::getenv("TEST_SRCDIR");
    const char * workspace = std::getenv("TEST_WORKSPACE");
    fs::path repo_path = "cthulhu/test_repo";
    if (runfiles_root && workspace) {
        repo_path = fs::path(runfiles_root) / workspace / "cthulhu/test_repo";
    }

    if (!fs::exists(repo_path)) {
        std::cerr << "FAIL: test repo not found at " << repo_path << "\n";
        return 1;
    }

    CharacterSheetRepo repo(repo_path.string());
    repo.load();
    expect(repo.players().size() == 3, "expected 3 players in test repo");
    expect(repo.get_character_sheet("Anna") != nullptr, "expected Anna sheet in test repo");

    const char * tmp_root = std::getenv("TEST_TMPDIR");
    fs::path tmp_dir = tmp_root ? fs::path(tmp_root) : fs::temp_directory_path();
    fs::path save_repo_path = tmp_dir / "roll_bot_test_repo";
    if (fs::exists(save_repo_path)) {
        fs::remove_all(save_repo_path);
    }
    fs::create_directories(save_repo_path);

    CharacterSheetRepo save_repo(save_repo_path.string());
    StatsT stats = {
        {"strength", Stat(50, false, StatType::ATTRIBUTE)},
        {"sanity", Stat(60, false, StatType::RESOURCE)},
    };
    save_repo.add(CharacterSheet("Bob", stats));
    save_repo.save();

    fs::path bob_sheet_path = save_repo_path / "Bob.json";
    expect(fs::exists(bob_sheet_path), "expected saved sheet file to exist");

    CharacterSheetRepo reload_repo(save_repo_path.string());
    reload_repo.load();
    expect(reload_repo.players().size() == 1, "expected one player after reload");
    expect(reload_repo.get_character_sheet("Bob") != nullptr, "expected Bob after reload");

    if (failures != 0) {
        std::cerr << failures << " test(s) failed\n";
    }
    return failures == 0 ? 0 : 1;
}
