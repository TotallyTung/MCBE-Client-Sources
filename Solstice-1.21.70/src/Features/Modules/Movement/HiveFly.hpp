//
// Created by alteik on 02/09/2024.
//
#pragma once
#include <Features/Modules/Module.hpp>



class HiveFly : public ModuleBase<HiveFly> {
public:

    NumberSetting mSpeed = NumberSetting("Speed", "flying speed", 60, 0, 100, 1);
    NumberSetting mTimer = NumberSetting("Timer", "The timer to set when this module is enabled", 3, 0, 40, 1);

    HiveFly() : ModuleBase("HiveFly", "silly hive fly", ModuleCategory::Movement, 0, false) {

        addSettings(&mSpeed, &mTimer);

        mNames = {
                {Lowercase, "hivefly"},
                {LowercaseSpaced, "hive fly"},
                {Normal, "HiveFly"},
                {NormalSpaced, "Hive Fly"}
        };
    }

    bool mCanFly;
    int mVeloTick;
    int mTicksToStay;
    bool mShouldStay;

    void Reset();
    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
};