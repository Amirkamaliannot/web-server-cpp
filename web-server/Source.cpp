#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <chrono>
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "WS2_32.lib")

#include "mSocket.h"




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
    std::string ipaddress = "127.0.0.1";


    mSocket listenSocket(ipaddress, port);
    SOCKET clientSocket;


    while (true) {


        // Accept a client socket
        clientSocket = accept(listenSocket.s, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed with error: " << WSAGetLastError() << std::endl;
            closesocket(listenSocket.s);
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
                std::cout << bytesReceived << std::endl;
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
