//
// Created by vastrakai on 7/2/2024.
//

#include "MinecraftPackets.hpp"

#include <SDK/SigManager.hpp>

std::shared_ptr<Packet> MinecraftPackets::createPacket(PacketID id)
{
    int packetID = static_cast<int>(id);
    uintptr_t daFunc = SigManager::MinecraftPackets_createPacket;
    return MemUtils::callFastcall<std::shared_ptr<Packet>>(daFunc, packetID);
}
