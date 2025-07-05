#pragma once
//
// Created by vastrakai on 8/24/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class Velocity : public ModuleBase<Velocity> {
public:
    NumberSetting Horizontal = NumberSetting("Horizontal Velocity", "The multiplier for the horizontal velocity", -2, 2, 0, 0.01);
    NumberSetting Vertical = NumberSetting("Vertical Velocity", "The multiplier for the vertical velocity", -2, 2, 0, 0.01);

    Velocity() : ModuleBase("Velocity", "Modifies knockback from attacks", "Movement", 0, false){
        AddSetting(&Horizontal);
        AddSetting(&Vertical);
    }

    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;
};