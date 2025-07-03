#pragma once
//
// Created by vastrakai on 7/10/2024.
//

#include <Features/Modules/Module.hpp>

class Scaffold : public ModuleBase<Scaffold> {
public:
    enum class RotateMode {
        None,
        Normal,
        Down,
        Backwards
    };

    enum class FlickMode {
        None,
        Combat,
        Always
    };

    enum class PlacementMode {
        Normal,
        Flareon
    };

    enum class ClickPosition {
        Normal,
        Bedrock
    };

    enum class SwitchMode {
        None,
        Full,
        Fake,
        Spoof
    };

    enum class SwitchPriority {
        First,
        Highest
    };

    enum class TowerMode {
        Vanilla,
        Velocity,
        Clip,
        Timer
    };

    enum class BlockHUDStyle {
        None,
        Solstice,
    };

    NumberSetting mPlaces = NumberSetting("Places", "The amount of blocks to place per tick", 1, 0, 20, 0.01);
    NumberSetting mRange = NumberSetting("Range", "The range at which to place blocks", 5, 0, 10, 0.01);
    NumberSetting mExtend = NumberSetting("Extend", "The distance to extend the placement", 3, 0, 10, 1);
    EnumSettingT<RotateMode> mRotateMode = EnumSettingT<RotateMode>("Rotate Mode", "The mode of rotation", RotateMode::Normal, "None", "Normal", "Down", "Backwards");
    EnumSettingT<FlickMode> mFlickMode = EnumSettingT<FlickMode>("Flick Mode", "The mode for block flicking", FlickMode::Combat, "None", "Combat", "Always");
    EnumSettingT<PlacementMode> mPlacementMode = EnumSettingT<PlacementMode>("Placement", "The mode for block placement", PlacementMode::Normal, "Normal", "Flareon");
    EnumSettingT<SwitchMode> mSwitchMode = EnumSettingT<SwitchMode>("Switch Mode", "The mode for block switching", SwitchMode::Full, "None", "Full", "Fake", "Spoof");
    EnumSettingT<SwitchPriority> mSwitchPriority = EnumSettingT<SwitchPriority>("Switch Prio", "The priority for block switching", SwitchPriority::First, "First", "Highest");
    BoolSetting mHotbarOnly = BoolSetting("Hotbar Only", "Whether or not to only place blocks from the hotbar", false);
    EnumSettingT<TowerMode> mTowerMode = EnumSettingT<TowerMode>("Tower Mode", "The mode for tower placement", TowerMode::Vanilla, "Vanilla", "Velocity", "Clip", "Timer");
    NumberSetting mTowerSpeed = NumberSetting("Tower Speed", "The speed for tower placement", 8.5, 0, 20, 0.01);
    NumberSetting mTimerSpeed = NumberSetting("Timer Speed", "The timer speed for tower placement", 20.0f, 20.f, 80.0f, 1.f);
    BoolSetting mFallDistanceCheck = BoolSetting("Fall Distance Check", "Whether or not to check fall distance before towering", false);
    BoolSetting mAllowMovement = BoolSetting("Allow Movement", "Whether or not to allow movement while towering", false);
    EnumSettingT<BlockHUDStyle> mBlockHUDStyle = EnumSettingT<BlockHUDStyle>("HUD Style", "The style for the block HUD", BlockHUDStyle::Solstice, "None", "Solstice");
    //BoolSetting mFlareonV2Placement = BoolSetting("Flareon V2", "Whether or not to use Flareon V2 placement", false);
    BoolSetting mAvoidUnderplace = BoolSetting("Avoid Underplace", "Whether or not to avoid underplacing", false);
    BoolSetting mFastClutch = BoolSetting("Fast Clutch", "Whether or not to use fast clutch", false);
    NumberSetting mClutchFallDistance = NumberSetting("Clutch Fall Dist", "The fall distance to clutch at", 3, 0, 20, 0.01);
    NumberSetting mCluchPlaces = NumberSetting("Clutch Places", "The amount of blocks to place per tick", 1, 0, 20, 0.01);
    BoolSetting mLockY = BoolSetting("Lock Y", "Whether or not to lock the Y position", false);
    BoolSetting mSwing = BoolSetting("Swing", "Whether or not to swing the arm", false);
    BoolSetting mTest = BoolSetting("Diagonal bypass", "Test", false);
    BoolSetting mFirstEvent = BoolSetting("First Event", "Patch for very standard way to detect scaffold", false);
    EnumSettingT<ClickPosition> mClickPosition = EnumSettingT<ClickPosition>("Click Position", "The mode of click position", ClickPosition::Normal, "Normal", "Bedrock");

    Scaffold() : ModuleBase("Scaffold", "Automatically places blocks below you", ModuleCategory::Player, 0, false) {
        addSettings(
            &mPlaces,
            &mRange,
            &mExtend,
            &mRotateMode,
            &mFlickMode,
            &mPlacementMode,
            &mSwitchMode,
            &mSwitchPriority,
            &mHotbarOnly,
            &mTowerMode,
            &mTowerSpeed,
            &mTimerSpeed,
            &mFallDistanceCheck,
            &mAllowMovement,
            &mBlockHUDStyle,
            //&mFlareonV2Placement,
            &mAvoidUnderplace,
            &mFastClutch,
            &mClutchFallDistance,
            &mCluchPlaces,
            &mLockY,
            &mSwing,
            &mTest,
            &mFirstEvent,
            &mClickPosition
        );

        VISIBILITY_CONDITION(mFlickMode, mRotateMode.mValue != RotateMode::None);

        VISIBILITY_CONDITION(mSwitchPriority, mSwitchMode.mValue != SwitchMode::None);
        VISIBILITY_CONDITION(mHotbarOnly, mSwitchMode.mValue != SwitchMode::None);
        VISIBILITY_CONDITION(mTowerSpeed, mTowerMode.mValue != TowerMode::Vanilla && mTowerMode.mValue != TowerMode::Timer);
        VISIBILITY_CONDITION(mTimerSpeed, mTowerMode.mValue == TowerMode::Timer);

        VISIBILITY_CONDITION(mClutchFallDistance, mFastClutch.mValue);
        VISIBILITY_CONDITION(mCluchPlaces, mFastClutch.mValue);

        mNames = {
            {Lowercase, "scaffold"},
            {LowercaseSpaced, "scaffold"},
            {Normal, "Scaffold"},
            {NormalSpaced, "Scaffold"}
        };

        gFeatureManager->mDispatcher->listen<RenderEvent, &Scaffold::onRenderEvent>(this);
    }

    float mStartY = 0;
    glm::vec3 mLastBlock = { 0, 0, 0 };
    int mLastFace = -1;
    bool mShouldRotate = false;
    uint64_t mLastSwitchTime = 0;
    int mLastSlot = -1;
    bool mShouldClip = false;


    // Tower stuff
    bool mIsTowering = false;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    bool tickPlace(class BaseTickEvent& event);
    void onRenderEvent(class RenderEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
    glm::vec3 getRotBasedPos(float extend, float yPos);
    glm::vec3 getPlacePos(float extend);

    std::string getSettingDisplay() override {
        return mRotateMode.mValues[mRotateMode.as<int>()];
    }


};