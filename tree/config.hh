#pragma once

#include <string>

#include <nlohmann/json.hpp>

nlohmann::json get_config();

std::string get_secret_key(const nlohmann::json & config);