#pragma once
//
// Created by vastrakai on 8/23/2023.
//
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Module.h>

class AutoSprint : public ModuleBase<AutoSprint> {
public:
    BoolSetting OmniSprint = BoolSetting("OmniSprint", "Sprint in all directions", false);
    BoolSetting Always = BoolSetting("Always", "Always sets sprint flags", false);

    AutoSprint() : ModuleBase("AutoSprint", "Automatically sprints.", "Movement", 0, false) {
        AddSetting(&OmniSprint);
        AddSetting(&Always);
    }

    void OnTick() override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
};
