//
// Created by vastrakai on 7/5/2024.
//

#include "CommandUtils.hpp"

#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Network/LoopbackPacketSender.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>
#include <SDK/Minecraft/Network/Packets/CommandRequestPacket.hpp>

#include "spdlog/spdlog.h"

void CommandUtils::executeCommand(const std::string& command)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    auto crq = MinecraftPackets::createPacket<CommandRequestPacket>();
    crq->mCommand = command;
    crq->mOrigin.mPlayerId = player->getRuntimeID();
    ClientInstance::get()->getPacketSender()->send(crq.get());
    spdlog::info("Sent command: {}", command);
}
