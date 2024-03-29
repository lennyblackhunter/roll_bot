#include <string>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include <dpp/nlohmann/json.hpp>

using nlohmann::json;

std::string get_config_path() {
    char * val;
    val = std::getenv("BOT_CONFIG_PATH");
    if (val != NULL) {
        return val;
    }
    return "config.json";
}

json get_config() {
    json configdocument;
    std::ifstream configfile(get_config_path(), configfile.in);
    configfile >> configdocument;
    return configdocument;
}