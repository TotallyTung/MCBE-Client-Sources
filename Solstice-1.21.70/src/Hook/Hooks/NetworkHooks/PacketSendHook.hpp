//
// Created by vastrakai on 6/28/2024.
//

#pragma once

#include <Hook/Hook.hpp>
#include <SDK/Minecraft/Network/Packets/Packet.hpp>

class PacketSendHook : public Hook {
public:
    PacketSendHook() : Hook() {
        mName = "LoopbackPacketSender::send";
    }

    static std::unique_ptr<Detour> mDetour;
    static inline int mCurrentTick = 0;
    static inline bool mApplyJump = false; // Used by scripting

    static void* onPacketSend(void* _this, class Packet* packet);
    static void sendInputPacket(std::shared_ptr<class PlayerAuthInputPacket> packet);
    static void sendPacket(class PlayerAuthInputPacket* packet);
    void init() override;
};

