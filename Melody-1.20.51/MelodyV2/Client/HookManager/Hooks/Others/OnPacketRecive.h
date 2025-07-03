/*#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../SDK/Classes/LoopbackPacketSender.h"
#include "../../../Client.h"

class PacketReceiveHook {
protected:
    using func_t = void(__fastcall*)(PacketHandlerDispatcherInstance*, void*, __int64*, Packet**);
    static inline func_t oFunc;

    static void PacketHandlerDispatcherInstance_handle(PacketHandlerDispatcherInstance* dispatcher, void* networkIdentifier, __int64* netEventCallback, Packet** packet) {
        LocalPlayer* localPlayer = mc.getLocalPlayer();
        bool shouldCancel = false;

        // Handle received packets
        if (packet && *packet) {
            PacketID packetId = (*packet)->getId();

            switch (packetId) {
            case PacketID::Text: {
                TextPacket* tPacket = (TextPacket*)*packet;
                if (tPacket->messageType == 1) {
                    // Handle received text packet
                    // You can add your custom logic here
                }
                break;
            }
                               // Add more cases for other packet types you want to handle
                               // case PacketID::SomeOtherPacket:
                               //     // Handle other packet types
                               //     break;
            }

            // Allow modules to handle the received packet
            client->moduleMgr->onReceivePacket(*packet, shouldCancel);
        }

        // Call the original function if the packet shouldn't be cancelled
        if (!shouldCancel) {
            oFunc(dispatcher, networkIdentifier, netEventCallback, packet);
        }
    }

public:
    static void init() {
        uintptr_t sigOffset = findSig("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 30 48 8B 01 48 8B F2 48 8B D9 48 8B 80 ? ? ? ? FF 15");
        if (!sigOffset)
            return;

        MemoryUtils::CreateHook("PacketReceiveHook", sigOffset, (void*)&PacketReceiveHook::PacketHandlerDispatcherInstance_handle, (void*)&oFunc);
    }
};*/



// all of this is a test idk if it does works or not...
