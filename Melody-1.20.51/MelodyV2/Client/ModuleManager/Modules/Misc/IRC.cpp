// Created by Tony on 2024-10-10 10:01:54

// IRC.cpp
#include "IRC.h"
#include "../../../../Utils/Utils.h"
#include <thread>

IRC::IRC() : Module("IRC", "Interact with IRC chat", Category::CLIENT) {
    // Add module settings here if needed
}

void IRC::onEnable() {
    connectToServer();
}

void IRC::onDisable() {
    if (connected) {
        sendMessage("QUIT :Disconnecting");
       // socket.Close();
        connected = false;
    }
}

void IRC::onTick() {
    if (connected) {
        receiveMessages();
    }
}

void IRC::connectToServer() {
    /*try {
        socket.ConnectAsync(winrt::Windows::Networking::HostName{ winrt::to_hstring(serverAddress) },
            //winrt::to_hstring(std::to_string(serverPort))).get();

        //writer = winrt::Windows::Storage::Streams::DataWriter(socket.OutputStream());
        reader = winrt::Windows::Storage::Streams::DataReader(socket.InputStream());

        sendMessage("NICK " + nickname);
        sendMessage("USER " + nickname + " 0 * :Melody Client User");
        sendMessage("JOIN " + channel);

        connected = true;

        // Start receiving messages in a separate thread
        std::thread([this]() { receiveMessages(); }).detach();
    }
    catch (winrt::hresult_error const& ex) {
        mc.getLocalPlayer()->displayClientMessage( winrt::to_string(ex.message()).c_str());
    }*/
}

void IRC::receiveMessages() {
    try {
        while (connected) {
           // auto size = reader.LoadAsync(1024).get();
            /*if (size > 0) {
                auto message = reader.ReadString(size);
                // Process the received message here
                // You might want to parse it and handle different IRC commands
                mc.getLocalPlayer()->displayClientMessage("IRC: %s", winrt::to_string(message).c_str());
            }*/
        }
    }
    catch (winrt::hresult_error const& ex) {
        mc.getLocalPlayer()->displayClientMessage(winrt::to_string(ex.message()).c_str());
        connected = false;
    }
}

void IRC::sendMessage(const std::string& message) {
    if (!connected) return;

    /*std::lock_guard<std::mutex> lock(sendMutex);
    try {
        writer.WriteString(winrt::to_hstring(message + "\r\n"));
        writer.StoreAsync().get();
    }
    catch (winrt::hresult_error const& ex) {
        mc.getLocalPlayer()->displayClientMessage(winrt::to_string(ex.message()).c_str());
    }*/
}