#include<fstream>
#include"configreader.hpp"
#include<iostream>

std::vector<backendServers> loadConfig(const std::string& configfile) {
	std::vector<backendServers> servers;

	std::ifstream file(configfile);

	if (!file.is_open()) {
		std::cerr << "Error opening file" << configfile << std::endl;
		return servers;
	}
	// parsing config file data to json
	json config;
	file >> config;

	//reading servers from the cofig file

	for (auto& server : config["servers"]) {
		backendServers newServer;
		newServer.ip = server["ip"];
		newServer.port = server["port"];
		newServer.isHealthy = false; //initial state
		servers.push_back(newServer);
	}
	return servers;

}