#include <dpp/dpp.h>
#include <dpp/nlohmann/json.hpp>
#include <iomanip>
#include <sstream>

#include "bot.h"
#include "calculator.hh"

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
            std::string line;
            std::getline(ss, line);
            auto node_builder = NodeBuilder<int>(
                    {
                        BinaryOperator<int>('k', roll_and_add, 5),
                        BinaryOperator<int>('+', [](int x, int y) { return x + y; },3),
                        BinaryOperator<int>('*', [](int x, int y) { return x * y; },4),
                        });
            auto my_node = node_builder.string_to_node(line);
            std::string answer;
            if (my_node) {
                answer = std::to_string(my_node->evaluate());
            }
            else {
                answer = "to nie jest poprawny napis";
            }
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