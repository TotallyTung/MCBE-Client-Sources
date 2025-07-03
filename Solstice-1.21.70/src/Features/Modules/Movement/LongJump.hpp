#pragma once
//
// Created by vastrakai on 8/4/2024.
//


class LongJump : public ModuleBase<LongJump> {
public:
    NumberSetting mSpeed = NumberSetting("Speed", "How fast you go", 1, 0, 40, 0.01);
    NumberSetting mHeight = NumberSetting("Height", "How high you go", 0.42, 0, 10, 0.01);
    BoolSetting mTimerBoost = BoolSetting("Timer Boost", "Boosts timer when jumping", false);
    NumberSetting mTimer = NumberSetting("Timer", "The timer to set when this module is enabled", 28.30, 0.01, 60, 0.01);
    BoolSetting mApplyJumpFlags = BoolSetting("Apply Jump Flags", "Applies jump flags to the player", true);
    BoolSetting mDisableModules = BoolSetting("Disable Modules", "Disables other modules bound to the same key", false);

    LongJump() : ModuleBase("LongJump", "Lets you jump long distances", ModuleCategory::Movement, 0, false) {
        addSettings(
            &mSpeed,
            &mHeight,
            &mTimerBoost,
            &mTimer,
            &mApplyJumpFlags,
            &mDisableModules
        );

        VISIBILITY_CONDITION(mTimer, mTimerBoost.mValue);

        mNames = {
            {Lowercase, "longjump"},
            {LowercaseSpaced, "long jump"},
            {Normal, "LongJump"},
            {NormalSpaced, "Long Jump"}
        };
    }

    bool mHasJumped = false;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
};