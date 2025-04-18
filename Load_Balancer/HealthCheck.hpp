#ifndef HEALTH_CHECK
#define HEALTH_CHECK
#include<vector>
#include<string>
#include "servers.hpp"

void healthCheckLoop(std::vector<backendServers>& servers);
bool checkHealth(const std::string& ip, int port);
#endif // !HEALTH_CHECK
