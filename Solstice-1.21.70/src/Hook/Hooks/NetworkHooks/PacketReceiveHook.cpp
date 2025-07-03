//
// Created by vastrakai on 7/2/2024.
//

#include "PacketReceiveHook.hpp"

#include <magic_enum.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/GameSession.hpp>
#include <SDK/Minecraft/MinecraftSim.hpp>
#include <Utils/MiscUtils/ColorUtils.hpp>
#include <SDK/Minecraft/Network/Packets/NetworkStackLatencyPacket.hpp>
#include <omp.h>

std::unordered_map<PacketID, std::unique_ptr<Detour>> PacketReceiveHook::mDetours;

void* PacketReceiveHook::onPacketSend(void* _this, void* networkIdentifier, void* netEventCallback, std::shared_ptr<Packet> packet)
{
    auto ofunc = PacketReceiveHook::mDetours[packet->getId()]->getOriginal<&PacketReceiveHook::onPacketSend>();

    NetworkIdentifier = networkIdentifier;

    auto actualEventCallback = ClientInstance::get()->getMinecraftSim()->getGameSession()->getEventCallback();
    if (actualEventCallback != netEventCallback) {
        return ofunc(_this, networkIdentifier, netEventCallback, packet);
    }

    auto holder = nes::make_holder<PacketInEvent>(packet, networkIdentifier, netEventCallback);
    gFeatureManager->mDispatcher->trigger(holder);
    if (holder->isCancelled()) {
        auto ofunc2 = PacketReceiveHook::mDetours[PacketID::NetworkStackLatency]->getOriginal<&PacketReceiveHook::onPacketSend>();
        packet = MinecraftPackets::createPacket<NetworkStackLatencyPacket>();

        return ofunc2(_this, networkIdentifier, netEventCallback, packet);
    }

    return ofunc(_this, networkIdentifier, netEventCallback, packet);
}

void PacketReceiveHook::handlePacket(std::shared_ptr<Packet> packet)
{
    if (!NetworkIdentifier) return;
    onPacketSend(packet->mDispatcher, NetworkIdentifier, ClientInstance::get()->getMinecraftSim()->getGameSession()->getEventCallback(), packet);
}

void PacketReceiveHook::init()
{
    static bool called = false;
    if (called) return;
    called = true;

    auto packetIds = magic_enum::enum_values<PacketID>();

    uint64_t start = NOW;
    spdlog::info("Hooking {} packets", packetIds.size());

    for (int i = 0; i < 0x136; i++) { // Fuck magic enum
        auto id = static_cast<PacketID>(i);
        auto packet = MinecraftPackets::createPacket(static_cast<PacketID>(i));
        if (!packet) continue;

        auto packetFunc = packet->mDispatcher->getPacketHandler();
        if (!packetFunc) {
            spdlog::warn("Failed to hook packet: {} (0x{:X})", magic_enum::enum_name<PacketID>(id), i);
            continue;
        }


        auto detour = std::make_unique<Detour>("PacketHandlerDispatcherInstance<" + std::string(magic_enum::enum_name<PacketID>(id)) + "Packet,0>::handle", reinterpret_cast<void*>(packetFunc), &onPacketSend, true);
        mDetours[id] = std::move(detour);
    };

    uint64_t timeTaken = NOW - start;

    spdlog::info("Successfully hooked {} packets in {}ms", mDetours.size(), timeTaken);
}