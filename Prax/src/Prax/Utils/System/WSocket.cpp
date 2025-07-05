//
// Created by Flash on 7/22/2023.
//

#include "WSocket.h"

#define DEFAULT_BUFLEN 1024
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdio>
#include <string>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
//https://learn.microsoft.com/en-us/windows/win32/winsock/complete-client-code


struct addrinfo* result = nullptr,
        * ptr = nullptr,
        hints;

SOCKET WSocket::Socket = INVALID_SOCKET;
WSAData WSocket::WSAData = {0};

HRESULT WSocket::Init(PCSTR port) {
    HRESULT iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &WSAData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %ld\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo("127.0.0.1", port, &hints, &result);
    if (iResult != 0) {
        Logger::Write("WSocket", Logger::LogType::Error, "getaddrinfo failed with error: %d", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        // Create a SOCKET for connecting to server
        Socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (Socket == INVALID_SOCKET) {
            Logger::Write("WSocket", Logger::LogType::Error, "Socket failed with error: %d", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect(Socket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(Socket);
            Socket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);


    if (Socket == INVALID_SOCKET) {
        Logger::Write("WSocket", Logger::LogType::Error, "Unable to connect to server");
        WSACleanup();
        return 1;
    }

    Logger::Write("WSocket", Logger::LogType::Info, "Connected to server");

    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Receive, nullptr, 0, nullptr);
    return 0;
}

void WSocket::Receive() {
    int iResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    do {
        iResult = recv(Socket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            Logger::Write("WSocket", Logger::LogType::Info, "Bytes received: %d", iResult);
            std::string str(recvbuf);
            Logger::Write("WSocket", Logger::LogType::Info, "Message: %s", str.c_str());
        } else if (iResult == 0) {
            Logger::Write("WSocket", Logger::LogType::Info, "Connection closed");
        } else {
            Logger::Write("WSocket", Logger::LogType::Error, "recv failed with error: %d", WSAGetLastError());
        }

    } while (iResult > 0);
}

void WSocket::Shutdown() {
    int iResult;

    // shutdown the connection since no more data will be sent
    iResult = shutdown(Socket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        Logger::Write("WSocket", Logger::LogType::Error, "Shutdown failed with error: %d", WSAGetLastError());
        closesocket(Socket);
        WSACleanup();
        return;
    }

    // cleanup
    closesocket(Socket);
    WSACleanup();
}
