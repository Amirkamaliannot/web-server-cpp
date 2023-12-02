#include "clientSocket.h"



Client::Client(SOCKET& clientSocket)
{
	Socket = clientSocket;

	int clientAddressLength = sizeof(clientAddress);
	getpeername(clientSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
	inet_ntop(AF_INET, &(clientAddress.sin_addr), clientIP, INET_ADDRSTRLEN);
	Port = clientAddress.sin_port;
}
