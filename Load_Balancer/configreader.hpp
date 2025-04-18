#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include<vector>
#include<string>
#include"servers.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

std::vector<backendServers> loadConfig(const std::string& configFile);

#endif