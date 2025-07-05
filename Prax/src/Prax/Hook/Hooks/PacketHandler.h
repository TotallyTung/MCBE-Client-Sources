#pragma once
//
// Created by vastrakai on 9/14/2023.
//


#include <Prax/Hook/Hook.h>
#include <Prax/SDK/Game/Network/Network.h>
#include <memory>
#include <map>
#include <Prax/SDK/Game/Minecraft.h>

class PacketHandler : public Hook {
public:
    PacketHandler() : Hook("PacketHandler") {
        InitOnConstruct = true;
    }

    static std::map<PacketID, std::unique_ptr<Detour>> PacketHandlerDetours;
    static void* NetworkIdentifier;
    // function to get a packet id's ofunc (
    //auto ofunc = PacketHandler::PacketHandlerDetours[(PacketID)packet->getId()]->GetFastcall<void*, void*, void*, void*, std::shared_ptr<Packet>>();
    static inline auto GetOfunc(PacketID id) {
        return PacketHandlerDetours[id]->GetFastcall<void*, void*, void*, void*, std::shared_ptr<Packet>>();
    }

    static inline auto GetOfunc(int id)
    {
        return GetOfunc(static_cast<PacketID>(id));
    }

    static inline void HandlePacket(std::shared_ptr<Packet> packet)
    {
        if (!NetworkIdentifier) return;
        auto ofunc = GetOfunc(packet->getId());
        ofunc(packet->packetHandlerDispatcher, NetworkIdentifier, Minecraft::ClientInstance->GetNetEventCallback(), packet);
    }



    void Init() override;
    void UnHook() override;
};
