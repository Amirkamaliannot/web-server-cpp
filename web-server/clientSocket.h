#pragma once
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "WS2_32.lib")


class Client
{
public:
	SOCKET Socket;
	int Port;
	char clientIP[INET_ADDRSTRLEN];
	sockaddr_in clientAddress;

	Client(SOCKET& clientSocket);

};

