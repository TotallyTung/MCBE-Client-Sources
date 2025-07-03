// Created by Tony on 2024-10-10 10:01:54

// IRC.h
#pragma once

#include "../Module.h"
#include <winrt/windows.networking.sockets.h>
#include <string>
#include <mutex>

namespace Sockets = winrt::Windows::Networking::Sockets;

class IRC : public Module {
private:
    Sockets::StreamSocket socket;
    winrt::Windows::Storage::Streams::DataWriter writer{ nullptr };
    winrt::Windows::Storage::Streams::DataReader reader{ nullptr };

    std::string serverAddress = "irc.example.com";
    int serverPort = 6667;
    std::string nickname = "MelodyUser";
    std::string channel = "#melodyclient";

    bool connected = false;
    std::mutex sendMutex;

    void connectToServer();
    void receiveMessages();
    void sendMessage(const std::string& message);

public:
    IRC();

    void onEnable() override;
    void onDisable() override;
    void onTick();

    bool isConnected() const { return connected; }
    void setNickname(const std::string& nick) { nickname = nick; }
    void setChannel(const std::string& chan) { channel = chan; }
};