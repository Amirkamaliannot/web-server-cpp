#pragma once

#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "WS2_32.lib")

#include "clientSocket.h"


class mSocket
{
    WSADATA wsaData;

    sockaddr_in s_in;
    int sin_family = AF_INET;
    
    std::string ip;

    bool WSA();
    bool creatSockaddr_in();
    bool creatSocket();
    bool bindSocket();
    bool listenSocket();

public:
    SOCKET s;
    int port;
    mSocket(std::string ip, int port);

    int getSendBufferSize();
    SOCKET accept_client();
};

