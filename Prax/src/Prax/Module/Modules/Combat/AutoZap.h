//
// Created by vastrakai on 10/18/2023.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class AutoZap : public ModuleBase<AutoZap> {
public:
    NumberSetting MovementCompensation = NumberSetting("Move Comp.", "Compensates for movement", 0.0f, 5.0f, 2.0f, 0.1f);
    AutoZap() : ModuleBase("AutoZap", "Automatically zaps entities around you", "Combat", 0, false) {
        AddSetting(&MovementCompensation);
        DisableOnDimensionSwitch = true;
    }

    static Vector3 LastPos;
    static Vector3 CurrentPos;

    void OnTick() override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
};