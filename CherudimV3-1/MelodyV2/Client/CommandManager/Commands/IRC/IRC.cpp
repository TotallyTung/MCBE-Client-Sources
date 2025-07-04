#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <iostream>

#include "../Command.h"
#include "../../CommandManager.h"
#include "../../../Client.h"



#pragma comment(lib,"ws2_32.lib")

IRC::IRC() : Command("IRC", "IRC Modules", { "internet", "talk" }, "<u got some stupititys>") {}

bool sendDiscordMessage(const std::string& message) {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        mc.DisplayClientMessage("Failed to initialize winsock.\n");
        return 1;
    }

    // Create socket
    SOCKET socketClient = socket(AF_INET, SOCK_STREAM, 0);
    if (socketClient == INVALID_SOCKET) {
        mc.DisplayClientMessage("Failed to create socket.\n");
        WSACleanup();
        return 1;
    }

    // Prepare server address structure
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(443);
    serverAddress.sin_addr.s_addr = inet_addr("discord.com");

    // Connect to the server
    if (connect(socketClient, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Connection error\n";
        closesocket(socketClient);
        WSACleanup();
        return false;
    }

    // message data w/ headers
    char request[1024];
    sprintf(request, "POST /api/webhooks/1231302759848874044/Ts9MY4l__LFP5kTSjJh61EUemabi-uPay8_es91yRaLJwuNPwUZW9ElLHCC94V-pjzIj HTTP/1.1\r\n"
        "Host: discord.com\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "{\"content\":\"Hello, Discord!\"}", strlen("{\"content\":\"Hello, Discord!\"}"));

    // Prepare HTTP request
    /*std::string httpRequest = "POST /api/webhooks/1231302759848874044/Ts9MY4l__LFP5kTSjJh61EUemabi-uPay8_es91yRaLJwuNPwUZW9ElLHCC94V-pjzIj HTTP/1.1\r\n";
    httpRequest += "Host: discordapp.com\r\n";
    httpRequest += "Content-Type: application/json\r\n";
    httpRequest += "Content-Length: " + std::to_string(message.size()) + "\r\n";
    httpRequest += "\r\n";
    httpRequest += message; */

    // Send HTTP request
    /*if (send(s, httpRequest.c_str(), httpRequest.size(), 0) < 0) {
        std::cerr << "Send failed\n";
        closesocket(s);
        WSACleanup();
        return false;
    } */

    // attempt to send the message
    if (send(socketClient, request, strlen(request), 0) < 0) {
        mc.DisplayClientMessage("Failed to send request.\n");
        closesocket(socketClient);
        WSACleanup();
        return 1;
    }

    char response[4096];
    if (recv(socketClient, response, sizeof(response), 0) < 0) {
        mc.DisplayClientMessage("Failed to receive response.\n");
        closesocket(socketClient);
        WSACleanup();
        return 1;
    }

    mc.DisplayClientMessage("Response:\n%s\n", response);

    // Close socket and cleanup


    closesocket(socketClient);
    WSACleanup();

    return 0;

    return true;
}

void IRC::execute(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        char errorMsg[] = "[%sMelody%s] %sInvalid command! Usage: .(IRC,internet,talk) words";
        mc.DisplayClientMessage(errorMsg, DARK_PURPLE, WHITE, RED);
        return;
    }

    std::string subCommand = args[1];

    switch (subCommand[0]) {
    case 'i':
        if (subCommand == "irc") {
            // Here we assume args[2] onwards contains the message to send
            std::string message = args[2];
            for (size_t i = 3; i < args.size(); ++i) {
                message += " " + args[i];
            }

            if (sendDiscordMessage(message)) {
                mc.DisplayClientMessage("[%sMelody%s] %sMessage sent to Discord webhook successfully!", DARK_PURPLE, WHITE, GREEN);
            }
            else {
                mc.DisplayClientMessage("[%sMelody%s] %sFailed to send message to Discord webhook.", DARK_PURPLE, WHITE, RED);
            }
        }
        break;
    }
}