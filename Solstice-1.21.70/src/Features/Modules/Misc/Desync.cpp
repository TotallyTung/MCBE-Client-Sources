//
// Created by vastrakai on 11/5/2024.
//

#include "Desync.hpp"

#include <Features/Events/PacketOutEvent.hpp>
#include <Hook/Hooks/NetworkHooks/PacketSendHook.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>

#ifdef __PRIVATE_BUILD__
#define ddebug(...) if (mDebug.mValue) ChatUtils::displayClientMessage(__VA_ARGS__)
#else
#define ddebug(...)
#endif

void Desync::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Desync::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &Desync::onPacketOutEvent>(this);
}

void Desync::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Desync::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &Desync::onPacketOutEvent>(this);
}


void Desync::onBaseTickEvent(BaseTickEvent& event)
{
    uint64_t delay = mMilliseconds.mValue;
    if (NOW - mLastSync > delay)
        syncPackets();
}

void Desync::syncPackets()
{
    for (auto& packet : mQueuedPackets)
    {
        PacketSendHook::sendInputPacket(packet);
    }

    ddebug("§6[Desync] §aSent §7" + std::to_string(mQueuedPackets.size()) + " §apackets since last sync");
    mLastSync = NOW;
    mQueuedPackets.clear();
}

void Desync::onPacketOutEvent(PacketOutEvent& event)
{
    if (event.mPacket->getId() == PacketID::PlayerAuthInput)
    {
        auto input = event.getPacket<PlayerAuthInputPacket>();
        auto clone = MinecraftPackets::createPacket<PlayerAuthInputPacket>();
        *clone = *input;
        event.cancel();
        mQueuedPackets.push_back(clone);
    }
}
