#pragma once

#include <string>

#include <dpp/nlohmann/json.hpp>

/* Config structure:
 *
 * {
 *  "repository_dir": "path/to/repository"
 * }
 */



nlohmann::json get_config();

std::string get_secret_key(const nlohmann::json & config);