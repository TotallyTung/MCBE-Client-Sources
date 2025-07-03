#pragma once
//
// Created by vastrakai on 6/28/2024.
//

#include <Utils/MemUtils.hpp>
/*Address of signature = Minecraft.Windows.exe + 0x01B2E0A0
"\x48\x83\xEC\x00\x48\x0F\x00\x00\x00\x48\x83\xC0\x00\x74\x00\x48\x83\xF8\x00\x48\x8B", "xxx?xx???xxx?x?xxx?xx"
"48 83 EC ? 48 0F ? ? ? 48 83 C0 ? 74 ? 48 83 F8 ? 48 8B"*/
class LoopbackPacketSender {
public:
    CLASS_FIELD(uintptr_t**, vtable, 0x0);
    CLASS_FIELD(class NetworkSystem*, mNetworkSystem, 0x20);

    virtual ~LoopbackPacketSender() = 0;
private:
    virtual bool isInitialized() = 0;
    // credit: tozic
public:
    /// <summary>
    /// Sends a packet to the server.
    /// Packets sent with this function will trigger the PacketOutEvent.
    /// </summary>
    /// <param name="packet">The packet to send.</param>
    virtual void send(void* packet) = 0;
    virtual void sendTo(void* packet) = 0;
    /// <summary>
    /// Sends a packet to the server.
    /// Calling this function will NOT trigger the PacketOutEvent.
    /// </summary>
    /// <param name="packet">The packet to send.</param>
    virtual void sendToServer(void* packet) = 0;

    // (none of the other funcs here matter)
};