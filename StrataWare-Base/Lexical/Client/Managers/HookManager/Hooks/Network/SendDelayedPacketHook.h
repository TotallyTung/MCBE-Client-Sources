#pragma once
#include "../FuncHook.h"
#include <queue>
#include <mutex>
/* NOT DONE, DO NOT USE */
class SendDelayedPacketHook : public FuncHook {
private:
    using func_t = void(__thiscall*)(PacketSender*, Packet*);
    static inline func_t oFunc;
    static inline std::queue<Packet*> canceledPackets;
    static inline std::mutex packetMutex;
    static void SendPacketCallback(PacketSender* _this, Packet* packet) {
        if (!packet) return;
        bool cancel = ModuleManager::onSendPacket(packet);
        if (cancel) {
            std::lock_guard<std::mutex> lock(packetMutex);
            canceledPackets.push(packet);
        }
        else {
            oFunc(_this, packet);
        }
    }
    static void SendCanceledPackets(PacketSender* _this) {
        std::lock_guard<std::mutex> lock(packetMutex);
        while (!canceledPackets.empty()) {
            Packet* packet = canceledPackets.front();
            canceledPackets.pop();
            oFunc(_this, packet);
        }
    }

public:
    SendDelayedPacketHook() {
        OriginFunc = (void**)&oFunc;
        func = (void*)&SendPacketCallback; 
    }
    static void ProcessCanceledPackets(PacketSender* _this) {
        SendCanceledPackets(_this); 
    }
};
