#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <functional>

#include "bot.h"
#include "commands.hh"
#include "calculator.hh"
#include "cthulhu/user_input.hh"
#include "cthulhu/character_sheet.hh"
#include "config.hh"


using json = nlohmann::json;

using MsgHandlerT = std::function<void(std::stringstream &, const dpp::message_create_t &, dpp::cluster &)>;

int main(int argc, char const *argv[]) {
//    json configdocument;
//    std::ifstream configfile("config.json");
//    configfile >> configdocument;
    CharacterSheetRepo repo{"/home/benhauer-adm/Nie_praca/roll_bot/roll_bot/tree/cthulhu/test/repo"};
    repo.load();

    using namespace std::placeholders;
    std::map<std::string, MsgHandlerT> command_map {
            {"!roll", std::bind(on_roll, repo, _1, _2, _3)},
    };

    /* Setup the bot */
    dpp::cluster bot("OTMzNDYwMjU0NDgwNTMxNTU2.Yeh2mw.L6V0IDIei7r7HvdGulB_mx3rtu4");

    /* Log event */
    bot.on_log([&bot](const dpp::log_t &event) {
        if (event.severity >= dpp::ll_debug) {
            std::cout << dpp::utility::current_date_time() << " [" << dpp::utility::loglevel(event.severity) << "] "
                      << event.message << "\n";
        }
    });

    /* Use the on_message_create event to look for commands */
    bot.on_message_create([&bot, &command_map](const dpp::message_create_t &event) {

        std::stringstream ss(event.msg.content);
        std::string command;
        ss >> command;
        command_map[command](ss, event, bot);
    });
    /* Start bot */
    bot.start(false);
    return 0;
};

//void configure_bot(dpp::cluster & bot, const json & config, CharacterSheetRepo & repo);

//CharacterSheetRepo get_repository(const json & config) {
//    return CharacterSheetRepo(config["repository_dir"]);
//}
//
//int main(int argc, char const *argv[]) {
//    json config = get_config();
//    dpp::cluster bot(get_secret_key(config));
//    CharacterSheetRepo repo = get_repository(config);
//    //configure_bot(bot, config, repo);
//    bot.start(false);
//    // Tu się zwiesi główny wątek w oczekiwaniu na polecenie zamknięcia programu i posprzątania po sobie
//    return 0;
//}
