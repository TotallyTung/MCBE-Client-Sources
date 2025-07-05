#pragma once
//
// Created by vastrakai on 9/23/2023.
//

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include <memory>

class DamageBoost : public ModuleBase<DamageBoost> {
public:
    NumberSetting SpeedSetting = NumberSetting("Speed", "How fast to go when you get hit", 0, 20, 7, 1);
    NumberSetting UpwardsMultiplier = NumberSetting("Upwards Multiplier", "How much to multiply the upwards velocity by", 0, 10, 1.f, 1);
    BoolSetting ApplyUpwardsVelocity = BoolSetting("Apply Upwards Velocity", "Applies upwards velocity when you get hit", true);
    BoolSetting AdjustForKnockback = BoolSetting("Adjust for Knockback", "Adjusts for the actual knockback you take", true);

    DamageBoost() : ModuleBase("DamageBoost", "Boosts you when you take damage", "Movement", 0, false) {
        SpeedSetting.Display = true;
        AddSetting(&SpeedSetting);
        AddSetting(&UpwardsMultiplier);
        AddSetting(&ApplyUpwardsVelocity);
        AddSetting(&AdjustForKnockback);
    }

    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;
};