#ifndef SERVER_HPP
#define SERVER_HPP

#include<string>
#include<vector>
#include<mutex>
struct backendServers {
	std::string ip;
	int port;
	bool isHealthy;
	int failCount = 0;
	int successCount = 0;
};
extern std::mutex serverMutex;
#endif // !SERVER_HPP

