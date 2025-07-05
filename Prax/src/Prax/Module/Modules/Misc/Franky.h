#pragma once
//
// Created by vastrakai on 9/17/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>

class Franky : public ModuleBase<Franky> {
public:

    enum InvalidMode {
        Blank,
        PosZero,
        PosMax,
    };
    NumberSetting InvalidTicks = NumberSetting("Invalid Ticks", "How many ticks to send invalid packets", 1, 100, 1, 1);
    ModeSetting InvalidMode = ModeSetting("Invalid Mode", "The mode to use for invalid packets", {"Blank", "PosZero", "PosMax"}, 0);

    Franky() : ModuleBase("Franky", "It does exactly what you think it does.", "Misc", 0, false) {
        InvalidTicks.Display = true;
        AddSetting(&InvalidTicks);
        AddSetting(&InvalidMode);
    };

    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) override;
    void OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled) override;
    void OnEnable() override;
    void OnDisable() override;


};
