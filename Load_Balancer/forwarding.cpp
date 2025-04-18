#include "forwarding.hpp"
#include<WinSock2.h>
#include<iostream>
using namespace std;


void forwardData(SOCKET fromSocket, SOCKET toSocket) {
    char buffer[4096];
    int bytesRead;

    while ((bytesRead = recv(fromSocket, buffer, sizeof(buffer), 0)) > 0) {
        send(toSocket, buffer, bytesRead, 0);
        cout << "[FORWARD] " << bytesRead << " bytes forwarded" << endl;
    }

    shutdown(fromSocket, SD_BOTH);
    shutdown(toSocket, SD_BOTH);
    closesocket(fromSocket);
    closesocket(toSocket);
}
