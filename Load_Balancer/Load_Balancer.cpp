#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include<windows.h>
#include <thread>
#include <vector>
#include "servers.hpp"
#include "HealthCheck.hpp"
#include "forwarding.hpp"
#include<mutex>
#include"configreader.hpp"
#include"AutomaticReload.hpp"

#pragma comment(lib, "Ws2_32.lib")
using namespace std;

int main() {
    WSADATA wsaData;
    int wResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wResult != 0) {
        cerr << "WSA startup failed!" << wResult << endl;
        return 1;
    }
    else {
        cout << "WSA startup successful!" << endl;
    }
	mutex serverMutex;

    vector<backendServers> server = loadConfig("config.json");
    if (server.empty()) {
        cerr << "error laoding servers from cofig" << endl;
        return 1;
    }

    // Start health checker thread
    thread healthThread(healthCheckLoop, ref(server));
    thread configWatcher(watchConfigFile, "Config.json", ref(server), ref(serverMutex));
    configWatcher.detach();

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Error creating Socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int result = ::bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        cerr << "Error binding socket: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Error listening on socket: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Load Balancer is listening on port 8080..." << endl;

    while (true) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Accept failed: " << WSAGetLastError() << endl;
            continue;
        }

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
        cout << "New connection from: " << clientIP << endl;

        // Pick a healthy backend server
		//Implemented Round Robin Scheduling
        static size_t currentServerIndex = 0;
        backendServers* targetBackend = nullptr;
        
        std::lock_guard<std::mutex> lock(serverMutex);
        for (size_t i = 0; i < server.size(); i++) { 
            size_t index = (currentServerIndex + i) % server.size();
			if (server[index].isHealthy) {
				targetBackend = &server[index];
				currentServerIndex = index; 
				break;
			}
        }
		if (!targetBackend) {
			cerr << "No healthy backend servers available." << endl;
			closesocket(clientSocket);
			continue;
		}

        // Connect to backend server
        SOCKET backendSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (backendSocket == INVALID_SOCKET) {
            cerr << "Failed to create backend socket." << endl;
            closesocket(clientSocket);
            continue;
        }

        sockaddr_in backendAddr;
        backendAddr.sin_family = AF_INET;
        backendAddr.sin_port = htons(targetBackend->port);
        inet_pton(AF_INET, targetBackend->ip.c_str(), &backendAddr.sin_addr);

        if (connect(backendSocket, (sockaddr*)&backendAddr, sizeof(backendAddr)) == SOCKET_ERROR) {
            cerr << "Failed to connect to backend server." << endl;
            closesocket(clientSocket);
            closesocket(backendSocket);
            continue;
        }

        cout << "Connected to backend: " << targetBackend->ip << ":" << targetBackend->port << endl;

        // Start data forwarding in both directions
        
        thread t1(forwardData, clientSocket, backendSocket); // client → backend
        thread t2(forwardData, backendSocket, clientSocket); // backend → client
        
        t1.detach();
        t2.detach();
       
    }

    healthThread.join();
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
