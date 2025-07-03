#pragma once
#include "../FuncHook.h"

class SendPacketHook : public FuncHook {
private:
    using func_t = void(__thiscall*)(PacketSender*, Packet*);
    static inline func_t oFunc;

    static void SendPacketCallback(PacketSender* _this, Packet* packet) {
        if (!packet) {
            return;
        }
        bool cancel = ModuleManager::onSendPacket(packet);
        if (!cancel) {
            oFunc(_this, packet);
        }
        else {
        }
    }

public:
    SendPacketHook() {
        OriginFunc = (void**)&oFunc;
        func = (void*)&SendPacketCallback;
    }
};
