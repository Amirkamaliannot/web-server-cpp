#include "mSocket.h"



bool mSocket::WSA()
{
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		//std::cerr << "WSAStartup failed: " << iResult << std::endl;
		return false;
	}
	return true;
}


bool mSocket::creatSockaddr_in()
{
	s_in.sin_family = sin_family;
	s_in.sin_port = htons(port);
	InetPtonA(AF_INET, "127.0.0.1", &s_in.sin_addr.s_addr);
	return true;
}

bool mSocket::creatSocket() {

	// Create a SOCKET for the server to listen for client connections
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET) {
		//Error creating socket
		WSACleanup();
		return false;
	}
	return true;
}

bool mSocket::bindSocket()
{
	if (bind(s, (LPSOCKADDR)&s_in, sizeof(s_in)) == SOCKET_ERROR)
	{
		//We couldn't bind (this will happen if you try to bind to the same socket more than once)
		return false;
	}
	return true;
}

bool mSocket::listenSocket()
{
	// Listen for incoming connection requests
    int iResult = listen(s, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        //Listen failed with error
        closesocket(s);
        WSACleanup();
        return false;
    }
	return true;
}


int mSocket::getSendBufferSize()
{
	int sendBufferSize;
	//int receiveBufferSize;
	int optlen = sizeof(sendBufferSize);

	// Get send buffer size
	if (getsockopt(s, SOL_SOCKET, SO_SNDBUF, (char*)&sendBufferSize, &optlen) == -1) {
	    std::cerr << "Error getting send buffer size: " << WSAGetLastError() << std::endl;
	    // Handle error here
	}
	//else {
	//    std::cout << "Send buffer size = " << sendBufferSize << " bytes" << std::endl;
	//}

	return sendBufferSize;
}

SOCKET mSocket::accept_client()
{
	if (s == INVALID_SOCKET) {
		std::cerr << "Broke socket can't Accept client: " << WSAGetLastError() << std::endl;
		return INVALID_SOCKET;
	}
	// Accept a client socket
	SOCKET clientSocket = accept(s, NULL, NULL);
	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
		return INVALID_SOCKET;
	}
	return clientSocket;
}

mSocket::mSocket(std::string, int port)
{

	this->ip = ip;
	this->port = port;

	if (WSA()) {
		if (creatSockaddr_in()) {
			if (creatSocket()) {
				if (bindSocket()) {

					if (listenSocket()) {
						std::cout << "Server listening on port " << port << std::endl;

					}
					else std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;

				}
				else std::cout << "Binding error !";

			}
			else std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
		}
	}
	else std::cout << "Error creating socket";

}


