#pragma once
//
// Created by vastrakai on 7/8/2024.
//

#include <Features/Modules/Module.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>


class Aura : public ModuleBase<Aura> {
public:
    enum class Mode {
        Single,
        Multi,
        Switch
    };

    enum class AttackMode {
        Earliest,
        Synched
    };

    enum class RotateMode {
        None,
        Normal,
        Flick
    };

    enum class SwitchMode {
        None,
        Full,
        Spoof
    };

    enum class BypassMode {
        None,
        FlareonV2,
        Raycast
    };

    EnumSettingT<Mode> mMode = EnumSettingT("Mode", "The mode of the aura", Mode::Switch, "Single", "Multi", "Switch");
    EnumSettingT<AttackMode> mAttackMode = EnumSettingT("Attack Mode", "The mode of attack", AttackMode::Earliest, "Earliest", "Synched");
    EnumSettingT<RotateMode> mRotateMode = EnumSettingT("Rotate Mode", "The mode of rotation", RotateMode::Normal, "None", "Normal", "Flick");
    BoolSetting mHeadYaw = BoolSetting("Head Yaw", "Whether or not to desync head yaw", false);
    EnumSettingT<SwitchMode> mSwitchMode = EnumSettingT("Switch Mode", "The mode of switching", SwitchMode::None, "None", "Full", "Spoof");
    EnumSettingT<BypassMode> mBypassMode = EnumSettingT("Bypass Mode", "The type of bypass", BypassMode::Raycast, "None", "FlareonV2", "Raycast");
    BoolSetting mAutoFireSword = BoolSetting("Auto Fire Sword", "Whether or not to automatically use the fire sword", false);
    BoolSetting mFireSwordSpoof = BoolSetting("Fire Sword Spoof", "Whether or not to spoof the fire sword", false);
    BoolSetting mHotbarOnly = BoolSetting("Hotbar Only", "Whether or not to only attack with items in the hotbar", false);
    BoolSetting mFistFriends = BoolSetting("Fist Friends", "Whether or not to fist friends", false);
    NumberSetting mRange = NumberSetting("Range", "The range at which to attack enemies", 5, 0, 10, 0.01);
    BoolSetting mDynamicRange = BoolSetting("Dynamic Range", "Sets the range to the specified value when not moving", false);
    NumberSetting mDynamicRangeValue = NumberSetting("Dynamic Value", "The value for the dynamic range", 3, 0, 10, 0.01);
    BoolSetting mRandomizeAPS = BoolSetting("Randomize APS", "Whether or not to randomize the APS", false);
    NumberSetting mAPS = NumberSetting("APS", "The amount of attacks per second", 10, 0, 20, 0.01);
    NumberSetting mAPSMin = NumberSetting("APS Min", "The minimum APS to randomize", 10, 0, 20, 0.01);
    NumberSetting mAPSMax = NumberSetting("APS Max", "The maximum APS to randomize", 20, 0, 20, 0.01);
    BoolSetting mInteract = BoolSetting("Interact", "Whether or not to interact with the target", false);
    BoolSetting mThrowProjectiles = BoolSetting("Throw Projectiles", "Whether or not to throw projectiles at the target", false);
    NumberSetting mThrowDelay = NumberSetting("Throw Delay", "The delay between throwing projectiles (in ticks)", 1, 0, 20, 0.01);
    BoolSetting mAutoBow = BoolSetting("Auto Bow", "Whether or not to automatically shoot the bow", false);
    BoolSetting mSwing = BoolSetting("Swing", "Whether or not to swing the arm", true);
    BoolSetting mSwingDelay = BoolSetting("Swing Delay", "Whether or not to delay the swing", false);
    NumberSetting mSwingDelayValue = NumberSetting("Swing Delay Value", "The delay between swings (in seconds)", 4.5f, 0.f, 10.f, 0.01f);
    BoolSetting mStrafe = BoolSetting("Strafe", "Whether or not to strafe around the target", true);
    BoolSetting mAttackThroughWalls = BoolSetting("Attack through walls", "Whether or not to attack through walls", true);
    BoolSetting mThirdPerson = BoolSetting("Third Person", "Whether or not switch to third-person camera view on enable", false);
    BoolSetting mThirdPersonOnlyOnAttack = BoolSetting("Only On Attack", "Switch to third-person view only when attacking", false);
    BoolSetting mVisuals = BoolSetting("Visuals", "Whether or not to render visuals around the target", true);
    NumberSetting mUpDownSpeed = NumberSetting("Up-Down Speed", "Speed of spheres rotate", 1.2, 0, 20, 0.01);
    NumberSetting mSpheresAmount = NumberSetting("Spheres Amount", "Amount of spheres to draw", 12, 0, 20, 1);
    NumberSetting mSpheresSizeMultiplier = NumberSetting("Spheres Size multiplier", "Multiplied size of spheres", 0.2, 0, 3, 0.01);
    NumberSetting mSpheresSize = NumberSetting("Spheres Size", "Size of spheres", 4, 0, 20, 0.01);
    NumberSetting mSpheresMinSize = NumberSetting("Spheres Min Size", "Min size of spheres", 2.60, 0, 20, 0.01);
    NumberSetting mSpheresRadius = NumberSetting("Spheres radius", "Distance from speheres to target", 0.9, 0, 2, 0.1);
    BoolSetting mDisableOnDimensionChange = BoolSetting("Auto Disable", "Whether or not to disable the aura on dimension change", true);
    BoolSetting mDebug = BoolSetting("Debug", "Whether or not to display debug information", false);

    Aura() : ModuleBase("Aura", "Automatically attacks nearby enemies", ModuleCategory::Combat, 0, false) {
        addSettings(
               /* &mUpDownSpeed,
                &mSpheresAmount,
                &mSpheresSizeMultiplier,
                &mSpheresSize,
                &mSpheresMinSize,
                &mSpheresRadius,*/
            &mMode,
            &mAttackMode,
            &mRotateMode,
            &mHeadYaw,
            &mSwitchMode,
            &mBypassMode,
#ifdef __PRIVATE_BUILD__
            &mAutoFireSword,
            &mFireSwordSpoof,
#endif
            &mHotbarOnly,
            &mFistFriends,
            &mVisuals,
            &mRange,
            &mDynamicRange,
            &mDynamicRangeValue,
            &mRandomizeAPS,
            &mAPS,
            &mAPSMin,
            &mAPSMax,
            //&mInteract, // TODO: Implement this
            &mThrowProjectiles,
            &mThrowDelay,
#ifdef __PRIVATE_BUILD__
            &mAutoBow,
#endif
            &mAttackThroughWalls,
            &mSwing,
            &mSwingDelay,
            &mSwingDelayValue,
            &mStrafe,
            &mThirdPerson,
            &mThirdPersonOnlyOnAttack,
            &mDisableOnDimensionChange,
            &mDebug
        );

        VISIBILITY_CONDITION(mAutoFireSword, mSwitchMode.mValue != SwitchMode::None);
        VISIBILITY_CONDITION(mFireSwordSpoof, mAutoFireSword.mValue);
        VISIBILITY_CONDITION(mAPS, !mRandomizeAPS.mValue);
        VISIBILITY_CONDITION(mAPSMin, mRandomizeAPS.mValue);
        VISIBILITY_CONDITION(mAPSMax, mRandomizeAPS.mValue);
        VISIBILITY_CONDITION(mThrowDelay, mThrowProjectiles.mValue);
        VISIBILITY_CONDITION(mDynamicRangeValue, mDynamicRange.mValue);

        VISIBILITY_CONDITION(mSwingDelay, mSwing.mValue);
        VISIBILITY_CONDITION(mSwingDelayValue, mSwingDelay.mValue && mSwing.mValue);

        VISIBILITY_CONDITION(mThirdPersonOnlyOnAttack, mThirdPerson.mValue);

        // vis conditions for visuals
        VISIBILITY_CONDITION(mUpDownSpeed, mVisuals.mValue);
        VISIBILITY_CONDITION(mSpheresAmount, mVisuals.mValue);
        VISIBILITY_CONDITION(mSpheresSizeMultiplier, mVisuals.mValue);
        VISIBILITY_CONDITION(mSpheresSize, mVisuals.mValue);
        VISIBILITY_CONDITION(mSpheresMinSize, mVisuals.mValue);
        VISIBILITY_CONDITION(mSpheresRadius, mVisuals.mValue);



        mNames = {
            {Lowercase, "aura"},
            {LowercaseSpaced, "aura"},
            {Normal, "Aura"},
            {NormalSpaced, "Aura"}
        };
    }

    AABB mTargetedAABB = AABB();
    bool mRotating = false;
    static inline bool sHasTarget = false;
    static inline Actor* sTarget = nullptr;
    static inline int64_t sTargetRuntimeID = 0;
    int64_t mLastSwing = 0;
    int64_t mLastTransaction = 0;
    int mLastSlot = 0;
    bool mIsThirdPerson = false;

    int getSword(Actor* target);
    bool shouldUseFireSword(Actor* target);
    void onEnable() override;
    void onDisable() override;
    void rotate(Actor* target);
    void shootBow(Actor* target);
    void throwProjectiles(Actor* target);
    void onRenderEvent(class RenderEvent& event);
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
    void onBobHurtEvent(class BobHurtEvent& event);
    void onBoneRenderEvent(class BoneRenderEvent& event);
    Actor* findObstructingActor(Actor* player, Actor* target);

    std::string getSettingDisplay() override {
        return mMode.mValues[mMode.as<int>()];
    }
};
