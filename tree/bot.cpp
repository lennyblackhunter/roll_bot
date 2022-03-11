#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <iomanip>
#include <sstream>
#include <cstdlib>

#include "bot.h"
#include "calculator.hh"
#include "cthulhu/user_input.hh"
#include "cthulhu/character_sheet.hh"
#include "config.hh"


using json = nlohmann::json;

int main(int argc, char const *argv[]) {
//    json configdocument;
//    std::ifstream configfile("config.json");
//    configfile >> configdocument;

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
    bot.on_message_create([&bot](const dpp::message_create_t &event) {

        std::stringstream ss(event.msg.content);
        std::string command;
        ss >> command;

        if (command == "!hello") {
            bot.message_create(dpp::message(event.msg.channel_id, "Hello to you too."));
        }
        else if (command == "!random") {
            int lower, upper;
            ss >> lower >> upper;
            bot.message_create(
                    dpp::message(
                            event.msg.channel_id,
                            std::to_string(random_number(lower, upper)))
            );
        }
        else if (command == "!roll") {
            auto request = request_from_string(ss);
            if (request_from_string(ss)) {
                std::string character_name = event.msg.member.nickname;

            }


            std::string answer;
            answer = "to nie jest poprawny napis";
            bot.message_create(
                    dpp::message(
                            event.msg.channel_id,
                            answer));
        }
    });
    /* Start bot */
    bot.start(false);
    return 0;
}

//void configure_bot(dpp::cluster & bot, const json & config, CharacterSheetRepo & repo);


int main(int argc, char const *argv[]) {
    json config = get_config();
    dpp::cluster bot(get_secret_key(config));
    //CharacterSheetRepo repo = get_repository(config);
    //configure_bot(bot, config, repo);
    bot.start(false);
    // Tu się zwiesi główny wątek w oczekiwaniu na polecenie zamknięcia programu i posprzątania po sobie
    return 0;
}
