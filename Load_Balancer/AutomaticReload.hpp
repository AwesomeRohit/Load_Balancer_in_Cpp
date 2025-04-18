#pragma once
#include<string>
#include<mutex>
#include<vector>
#include<chrono>
#include<thread>
#include"servers.hpp"
#include"configreader.hpp"
#include<filesystem>
#include<iostream>

using namespace std;

void watchConfigFile(const string& filename, vector<backendServers>& servers, mutex& serverMutex) {

	namespace fs = std::filesystem;
	auto lastWriteTime = fs::last_write_time(filename);
    
	while (true) {
		
		this_thread::sleep_for(chrono::seconds(5));
		auto currentWriteTime = fs::last_write_time(filename);

		if (currentWriteTime != lastWriteTime) {
			lastWriteTime = currentWriteTime;
			vector<backendServers> newServers = loadConfig(filename);

			if (newServers.empty()) {
				lock_guard<mutex> lock(serverMutex);
				servers = newServers;
				 cout<< "reloaded servers succesfully" << endl;

			}
			else {
				cerr << "Error loading servers" << endl;
			}
		
		}

	}
}
