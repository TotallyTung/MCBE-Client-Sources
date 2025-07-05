//
// Created by vastrakai on 1/28/2024.
//

#include "SevenDay.h"

#include <Prax/Hook/Hooks/PacketSender.h>
#include <Prax/Utils/Math.h>
#include <Prax/SDK/Game/Network/Other/InteractPacket.h>

std::thread SevenDay::thread;



void SevenDay::OnRun(std::vector<std::string> args)
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    for (int i = 0; i < 60; i++)
    {
        auto paip = player->getInputPacket();
        // Randomize rotation between -FLT_MAX and FLT_MAX
        paip->rotation.x = -180;

        if (!Minecraft::ClientInstance->packetSender) return;
        bool canc = false;
        PacketSender::HandleAuthInputTick(paip.get(), &canc);
        Minecraft::ClientInstance->packetSender->sendToServer(paip.get());

        auto interactPacket = MinecraftPackets::CreatePacket<InteractPacket>();
        interactPacket->mAction = InteractPacket::Action::OpenInventory;
        interactPacket->mTargetId = player->GetRuntimeID();
        Minecraft::ClientInstance->packetSender->sendToServer(interactPacket.get());
    }

    AddCommandMessage("§bSeven times, seven times§7. §cSeven dayed, seven dayed.");
}
