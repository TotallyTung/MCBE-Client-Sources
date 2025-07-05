//
// Created by vastrakai on 10/28/2023.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>

class AntiImmobile : public ModuleBase<AntiImmobile> {
public:
    enum Mode {
        ClientFalse,
        Clip,
        HiveSky
    };

    ModeSetting Mode = ModeSetting("Mode", "The mode to use", { "Client False", "Clip", "Hive Sky" }, ClientFalse);

    AntiImmobile() : ModuleBase("AntiImmobile", "Prevents you from being immobile.", "Movement", 0, false) {
        AddSetting(&Mode);
        Mode.Display = true;
    }

    static uint64_t LastDimensionChange;
    static uint64_t LastTeleport;

    void OnTick() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) override;
};
