#include "HealthCheck.hpp"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <thread>
#include <chrono>
#include"servers.hpp"
#include<mutex>
#include"nlohmann/json.hpp"

std::mutex serverMutex;

bool checkHealth(const std::string& ip, int port) {
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) return false;

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    bool success = connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) != SOCKET_ERROR;

    closesocket(sock);
    return success;
}

void healthCheckLoop(std::vector<backendServers>& servers) {
    const int FAILURE_THRESHOLD = 3;
    const int SUCCESS_THRESHOLD = 2;

    while (true) {
		
        
        for (auto& server : servers) {
            bool success = checkHealth(server.ip, server.port);
            std::cout << "Checking " << server.ip << ":" << server.port << " - " << (success ? "OK" : "FAIL") << std::endl;
            
            std::lock_guard<std::mutex> lock(serverMutex);
            if (success) {
                server.successCount++;
                server.failCount = 0;

                if (!server.isHealthy && server.successCount >= SUCCESS_THRESHOLD) {
                    server.isHealthy = true;
                    std::cout << "[+] " << server.ip << ":" << server.port << " marked as HEALTHY\n";
                }
            }
            else {
                server.failCount++;
                server.successCount = 0;

                if (server.isHealthy && server.failCount >= FAILURE_THRESHOLD) {
                    server.isHealthy = false;
                    std::cout << "[-] " << server.ip << ":" << server.port << " marked as UNHEALTHY\n";
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
