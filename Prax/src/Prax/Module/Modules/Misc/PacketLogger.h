#pragma once
//
// Created by vastrakai on 9/14/2023.
//


#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Module.h>

class PacketLogger : public ModuleBase<PacketLogger> {
public:
    BoolSetting LogIncoming = BoolSetting("Log Incoming", "Log incoming packets", true);
    BoolSetting LogOutgoing = BoolSetting("Log Outgoing", "Log outgoing packets", true);

    PacketLogger() : ModuleBase("PacketLogger", "Logs packets", "Misc", 0, false) {
        AddSetting(&LogIncoming);
        AddSetting(&LogOutgoing);
    };

    static std::vector<int> ExcludedPacketIds;

    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) override;
    void OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled) override;


};