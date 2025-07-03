#pragma once
//
// Created by vastrakai on 7/1/2024.
//

#include <Features/Modules/Module.hpp>


class PacketLogger : public ModuleBase<PacketLogger>
{
public:
    BoolSetting mIncoming = BoolSetting("Incoming", "Log incoming packets", true);
    BoolSetting mOutgoing = BoolSetting("Outgoing", "Log outgoing packets", true);
    PacketLogger() : ModuleBase("PacketLogger", "Logs packets", ModuleCategory::Misc, 0, false)
    {
        addSettings(&mIncoming, &mOutgoing);
        mNames = {
            {Lowercase, "packetlogger"},
            {LowercaseSpaced, "packet logger"},
            {Normal, "PacketLogger"},
            {NormalSpaced, "Packet Logger"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onPacketOutEvent(class PacketOutEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
};