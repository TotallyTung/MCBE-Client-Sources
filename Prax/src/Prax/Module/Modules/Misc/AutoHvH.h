//
// Created by vastrakai on 4/14/2024.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/ModuleManager.h>

class AutoHvH : public ModuleBase<AutoHvH> {
public:
    AutoHvH() : ModuleBase("AutoHvH", "Automatically checks for other cheaters", "Misc", 0, false) {

    }

    int64_t LastDimensionChange = 0;

    void OnGameStart();

    void OnTick() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) override;
};