//
// Created by vastrakai on 1/6/2024.
//

#include "Hivekick.h"

#include <Prax/Hook/Hooks/PacketSender.h>
#include <Prax/SDK/Game/Entity/Actor.h>

// Static thread that wont get destroyed when the function ends
std::thread Hivekick::thread;

void Hivekick::OnRun(std::vector<std::string> args)
{
    // Create a new thread
    thread = std::thread([this]()
    {
        for (int i = 0; i < 4; i++) {
            auto player = Minecraft::ClientInstance->ThePlayer();
            if (!player) return;

            PacketSender::CurrentTick = 1; // This will get you kicked for BadTick
            for (int i = 0; i < 40; i++) {
                player->sendInput();
                PacketSender::CurrentTick = 1;
            }
            Sleep(300);
        }


    });

    thread.detach();
    AddCommandMessage("You will be kicked in 2 seconds!");
}
