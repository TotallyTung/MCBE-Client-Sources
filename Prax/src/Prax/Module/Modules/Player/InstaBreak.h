//
// Created by Flash on 7/11/2023.
//

#pragma once


#include "Prax/Module/Module.h"
#include "Prax/Module/Settings/ModeSetting.h"

class InstaBreak : public ModuleBase<InstaBreak> {
public:
    ModeSetting Mode = ModeSetting("Mode", "The mode to use", {"Vanilla", "DestroySpeed",  "ExtraPackets"}, 1);

    InstaBreak() : ModuleBase("InstaBreak", "Instantly breaks blocks", "Player", 0, false) {
        Mode.Display = true;
        AddSetting(&Mode);
    }

    void OnDisable() override;
    void OnTick() override;
};