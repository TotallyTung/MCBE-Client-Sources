#pragma once
//
// Created by vastrakai on 8/30/2023.
//


#include <Prax/Module/Module.h>

class AutoTool : public ModuleBase<AutoTool> {
public:
    AutoTool() : ModuleBase("AutoTool", "Automatically switches to the best tool for the block you're mining", "Player", 0, false) {};

    static int OldSlot;

    void OnDisable() override;
    void OnTick() override;
};