#include <thread>
#include <atomic>
#include <sstream>
#include <functional>

#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>

#include "config.hh"
#include "commands.hh"
#include "calculator.hh"
#include "cthulhu/character_sheet.hh"

using json = nlohmann::json;

using MsgHandlerT = std::function<void(std::stringstream &, const dpp::message_create_t &, dpp::cluster &)>;

static volatile bool button = true;

void sig_handler(int) {
    button = false;
};

int main(int argc, char const *argv[]) {
    json configdocument = get_config();

    std::signal(SIGINT, sig_handler);
    std::signal(SIGTERM, sig_handler);

    std::atomic<CharacterSheetRepo*> repo(
            new CharacterSheetRepo(configdocument["path_to_repo"])
                    );
    
    repo.load()->load();
    using namespace std::placeholders;
    std::map<std::string, MsgHandlerT> command_map {
            {"!roll", [&](std::stringstream & x, const dpp::message_create_t & y, dpp::cluster & z){on_roll(repo, x, y, z);}},
            {"!stat", [&](std::stringstream & x, const dpp::message_create_t & y, dpp::cluster & z){on_set_stat(repo, x, y, z);}},
            {"!sheet", [&](std::stringstream & x, const dpp::message_create_t & y, dpp::cluster & z){on_sheet_request(repo, x, y, z);}},
            {"!goodbye", std::bind(on_turn_off, &button, _1, _2, _3)},
    };

    /* Set up the bot */
    dpp::cluster bot(configdocument["token"], dpp::i_default_intents | dpp::i_message_content);

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
    bot.start(true);
    while (button) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    };
    repo.load()->save();
    return 0;
};
