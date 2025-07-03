#pragma once
//
// Created by vastrakai on 7/2/2024.
//

#include <Hook/Hook.hpp>
#include <SDK/Minecraft/Network/PacketID.hpp>

class PacketReceiveHook : public Hook {
public:
    PacketReceiveHook() : Hook() {
        mName = "PacketReceiveHook";
    }

    static std::unordered_map<PacketID, std::unique_ptr<Detour>> mDetours;
    static inline void* NetworkIdentifier = nullptr;

    static void* onPacketSend(void* _this, void* networkIdentifier, void* netEventCallback, std::shared_ptr<class Packet> packet);
    static void handlePacket(std::shared_ptr<Packet> packet);
    void init() override;
};

