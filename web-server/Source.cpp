#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <chrono>
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "WS2_32.lib")




std::string readHTML() {
    std::ifstream t("./html/index.html");
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    return str;
}

std::string creatHTML() {

    std::string content = readHTML();
    std::string http_header = "HTTP/1.1 200 OK\r\n"
        "Date: Sun, 18 Oct 2012 10:36:20 GMT\r\n"
        "Server: Apache/2.2.14 (Win32)\r\n"
        "Content-Length: " + std::to_string(content.size()) +"\r\n"
        "Content-Type: text/html; charset=iso-8859-1\r\n"
        "Connection: Closed\r\n\r\n";

    return http_header + content;
}



int main()
{
    int port;
    std::cout << "Enter the port : ";
    std::cin >> port;

    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    sockaddr_in s_in;
    s_in.sin_family = AF_INET;
    s_in.sin_port = htons(port);
    InetPton(AF_INET, L"127.0.0.1", &s_in.sin_addr.s_addr);


    SOCKET listenSocket = INVALID_SOCKET;
    // Create a SOCKET for the server to listen for client connections
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }


    if (bind(listenSocket, (LPSOCKADDR)&s_in, sizeof(s_in)) == SOCKET_ERROR)
    {
        //We couldn't bind (this will happen if you try to bind to the same  
        //socket more than once)

        std::cout << "Binding error";
        return false;
    }


    // Listen for incoming connection requests
    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "Listen failed with error: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }


    std::cout << "Server listening on port " << port << std::endl;

    SOCKET clientSocket;


    int sendBufferSize;
    int receiveBufferSize;
    int optlen = sizeof(sendBufferSize);

    // Get send buffer size
    if (getsockopt(listenSocket, SOL_SOCKET, SO_SNDBUF, (char*)&sendBufferSize, &optlen) == -1) {
        std::cerr << "Error getting send buffer size: " << WSAGetLastError() << std::endl;
        // Handle error here
    }
    else {
        std::cout << "Send buffer size = " << sendBufferSize << " bytes" << std::endl;
    }


    while (true) {


        // Accept a client socket
        clientSocket = accept(listenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }
        else {
            // Get the client IP address
            sockaddr_in clientAddress{};
            int clientAddressLength = sizeof(clientAddress);
            getpeername(clientSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(clientAddress.sin_addr), clientIP, INET_ADDRSTRLEN);
            std::cout << "Request Received from " << clientIP << std::endl;
        }
        char recvBuffer[1024];
        int bytesReceived;
        // Receive and handle incoming data from the client

        //while (true) {
            bytesReceived = recv(clientSocket, recvBuffer, 1024, 0);
            if (bytesReceived > 0) {
                // Handle received data here
                // Echo the received data back to the client

                std::string response = creatHTML();
                std::cout << response.length() << std::endl;
                send(clientSocket, response.c_str(), response.length(), 0);
                closesocket(clientSocket);
            }
            else if (bytesReceived == 0) {
                // Client disconnected
                std::cout << "Client disconnected." << std::endl;
                break;
            }
            else {
                std::cerr << "Receive failed with error: " << WSAGetLastError() << std::endl;
                closesocket(clientSocket);
                WSACleanup();
                return 1;
            }
        //}
    }
}
