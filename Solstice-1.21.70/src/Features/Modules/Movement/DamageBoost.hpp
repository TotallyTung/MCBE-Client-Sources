#pragma once
//
// Created by ssi on 10/26/2024.
//

#include <Features/Modules/Module.hpp>

class DamageBoost : public ModuleBase<DamageBoost> {
public:
    NumberSetting mSpeed = NumberSetting("Speed", "The Speed boost applied after taking damage", 8.f, 1.f, 20.0f, 0.01f);
    BoolSetting mOnGroundCheck = BoolSetting("OnGround Check", "Different Speed boost when on ground or in air", false);
    NumberSetting mGroundSpeed = NumberSetting("Ground Speed", "Speed when on ground", 7.f, 1.f, 20.0f, 0.01f);
    NumberSetting mOffGroundSpeed = NumberSetting("Off Ground Speed", "Speed when in air", 10.f, 1.f, 20.0f, 0.01f);

    DamageBoost() : ModuleBase("DamageBoost", "Boosts your speed when taking damage", ModuleCategory::Movement, 0, false) {
        addSettings(&mSpeed, &mOnGroundCheck, &mGroundSpeed, &mOffGroundSpeed);
        VISIBILITY_CONDITION(mGroundSpeed, mOnGroundCheck.mValue);
        VISIBILITY_CONDITION(mOffGroundSpeed, mOnGroundCheck.mValue);

        mNames = {
            {Lowercase, "damageboost"},
            {LowercaseSpaced, "damage boost"},
            {Normal, "DamageBoost"},
            {NormalSpaced, "Damage Boost"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onPacketInEvent(class PacketInEvent& event);
};