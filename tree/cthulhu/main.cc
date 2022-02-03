#include <dpp/dpp.h>
#include <iostream>
 
int main()
{
    dpp::cluster bot("NzkxMDYyOTE3ODIyMjgzNzg2.X-JswA.bi2XN7A3NqezwMKAiWGObWElJyE");
 
    bot.on_ready([&bot](const auto & event) {
        std::cout << "Logged in as " << bot.me.username << "!\n";
    });
 
    bot.on_message_create([&bot](const auto & event) {
        if (event.msg.content == "!ping") {
            bot.message_create(dpp::message(event.msg.channel_id, "Pong!"));
        }
    });
 
    bot.start(false);
    return 0;
}

