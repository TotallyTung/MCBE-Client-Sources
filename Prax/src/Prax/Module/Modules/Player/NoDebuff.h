//
// Created by vastrakai on 5/9/2024.
//


#pragma once

#include <Prax/Module/Module.h>

#include "Prax/SDK/Game/Network/Other/MobEffectPacket.h"

class NoDebuff : public ModuleBase<NoDebuff> {
public:
    BoolSetting NoSlowness = BoolSetting("NoSlowness", "Prevents Slowness from being applied to you.", true);
    BoolSetting NoMiningFatigue = BoolSetting("NoMiningFatigue", "Prevents Mining Fatigue from being applied to you.", true);
    BoolSetting NoBlindness = BoolSetting("NoBlindness", "Prevents Blindness from being applied to you.", true);
    BoolSetting NoNausea = BoolSetting("NoNausea", "Prevents Nausea from being applied to you.", true);
    BoolSetting NoWeakness = BoolSetting("NoWeakness", "Prevents Weakness from being applied to you.", true);

    NoDebuff() : ModuleBase("NoDebuff", "Prevents debuffs from being applied to you.", "Player", 0, false)
    {
        AddSetting(&NoSlowness);
        AddSetting(&NoMiningFatigue);
        AddSetting(&NoBlindness);
        AddSetting(&NoNausea);
        AddSetting(&NoWeakness);
    }

    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) override;
};
