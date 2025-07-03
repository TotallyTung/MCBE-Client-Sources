#pragma once
//
// Created by dark on 9/19/2024.
//

#include <Features/Modules/Module.hpp>
#include <Features/Modules/Setting.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>

class TargetStrafe : public ModuleBase<TargetStrafe> {
public:
    NumberSetting mDistance = NumberSetting("Distance", "Distance to target", 3.f, 0.5f, 5.f, 0.1f);
    NumberSetting mMinDistance = NumberSetting("Min Distance", "Minimum distance to target", 3.f, 0.5f, 5.f, 0.1f);
    BoolSetting mWallCheck = BoolSetting("Wall Check", "Whether or not to switch direction when you collide block", false);
    BoolSetting mJumpOnly = BoolSetting("Jump Only", "Whether or not to strafe only while jumping", false);
    BoolSetting mSpeedOnly = BoolSetting("Speed Only", "Whether or not to strafe only while speed enabled", false);
    BoolSetting mAlwaysSprint = BoolSetting("Always Sprint", "Whether or not to always sprint", false);
    BoolSetting mRenderCircle = BoolSetting("Render Circle", "Whether or not to switch direction when you collide block", false);

    TargetStrafe() : ModuleBase("TargetStrafe", "Strafes around your target", ModuleCategory::Movement, 0, false) {
        addSettings(
            &mDistance,
            &mMinDistance,
            &mWallCheck,
            &mJumpOnly,
            &mSpeedOnly,
            &mAlwaysSprint
            //&mRenderCircle // TODO
        );

        mNames = {
            {Lowercase, "targetstrafe"},
            {LowercaseSpaced, "target strafe"},
            {Normal, "TargetStrafe"},
            {NormalSpaced, "Target Strafe"}
        };
    }

    bool mShouldStrafe = false;
    Actor* mCurrentTarget = nullptr;

    bool mForward = false;
    bool mBackward = false;
    bool mMoveRight = true;


    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onRenderEvent(class RenderEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
    void handleKeyInput(bool pressingW, bool pressingA, bool pressingS, bool pressingD);
};