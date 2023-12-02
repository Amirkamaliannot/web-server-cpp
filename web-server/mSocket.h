#pragma once

#include <iostream>
#include <sys/types.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <cstring>
#pragma comment(lib, "WS2_32.lib")


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
};

