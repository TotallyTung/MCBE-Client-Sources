//
// Created by alteik on 04/10/2024.
//
#include <Features/Modules/Module.hpp>
#include <Features/Modules/Setting.hpp>
#pragma once

class Criticals : public ModuleBase<Criticals> {
public:

    enum class Mode {
        Sentinel,
    };

    enum class AnimationState {
        START,
        MID_AIR,
        MID_AIR2,
        LANDING,
        FINISHED
    };

    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The critical mode", Mode::Sentinel, "Sentinel");
    BoolSetting mVelocity = BoolSetting("Use Velocity", "Whether to use velocity change", true);
    BoolSetting mPositionChange = BoolSetting("Position Change", "Whether to change pos", true);
    BoolSetting mBiggerPositionChange = BoolSetting("Bigger Position Change", "Whether to change more pos", true);
    BoolSetting mSendJumping = BoolSetting("Send Jumping", "Whether to send jumping", true);
    BoolSetting mOffSprint = BoolSetting("Disable Sprint", "Whether to disable sprint", true);
    NumberSetting mPositionChangePersent = NumberSetting("Position Change Percent", "Changes delta", 1.5, 0, 2, 0.01);

    Criticals() : ModuleBase("Criticals", "Makes you always get a critical hit on your opponent", ModuleCategory::Combat, 0, false) {

        addSetting(&mMode);
        addSetting(&mVelocity);
        addSetting(&mPositionChange);
        addSetting(&mBiggerPositionChange);
        addSetting(&mSendJumping);
        addSetting(&mOffSprint);
        addSetting(&mPositionChangePersent);

        mNames = {
                {Lowercase, "criticals"},
                {LowercaseSpaced, "criticals"},
                {Normal, "Criticals"},
                {NormalSpaced, "Criticals"}
        };
    }

    bool mWasSprinting = true;
    AnimationState mAnimationState = AnimationState::START;

    float mJumpPositions[4] = {0, 0.8200100660324097 - 0.6200100183486938, 0.741610050201416 - 0.6200100183486938, 0}; // glide down with 0.01 persision
    float mJumpPositionsMini[4] = {0, 0.02, 0.01, 0}; // 0.05
    float mJumpVelocities[4] = {-0.07840000092983246, -0.07840000092983246, -0.1552319973707199f, -0.07840000092983246};

    void onEnable() override;
    void onDisable() override;
    void onPacketOutEvent(class PacketOutEvent& event);
};