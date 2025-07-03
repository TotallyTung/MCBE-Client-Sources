#pragma once
//
// Created by vastrakai on 7/22/2024.
//

#include <Features/Modules/Module.hpp>

class Derp : public ModuleBase<Derp>
{
public:
    enum class Mode
    {
        None,
        Spin,
        Random,
        Headroll
    };

    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode of the derp", Mode::None, "None", "Spin", "Random", "Headroll");
    NumberSetting mSpeed = NumberSetting("Speed", "The speed of the derp", 1.0f, 0.1f, 10.0f, 0.01f);
    BoolSetting mHeadOnly = BoolSetting("Head Only", "Whether to rotate the head yaw only", false);
    BoolSetting mHeadYawDesync = BoolSetting("Head Yaw Desync", "Whether to desync the head yaw", false);
    BoolSetting mHeadYawFlip = BoolSetting("Head Yaw Flip", "Whether to flip the head yaw", false);
    NumberSetting mFlipTick = NumberSetting("Flip Tick", "The tick to flip the head yaw", 20.0f, 1.0f, 20.0f, 1.0f);
    BoolSetting mOffGroundOnly = BoolSetting("Off Ground Only", "Rotates only when you are not on the ground", false);

    Derp() : ModuleBase("Derp", "Makes you look like a derp", ModuleCategory::Player, 0, false)
    {
        addSettings(&mMode, &mSpeed, &mHeadOnly, &mHeadYawDesync, &mHeadYawFlip, &mFlipTick, &mOffGroundOnly);

        VISIBILITY_CONDITION(mSpeed, mMode.mValue != Mode::None);
        VISIBILITY_CONDITION(mHeadOnly, mMode.mValue == Mode::Spin);
        VISIBILITY_CONDITION(mHeadYawFlip, mHeadYawDesync.mValue);
        VISIBILITY_CONDITION(mFlipTick, mHeadYawFlip.mValue && mHeadYawDesync.mValue);
        VISIBILITY_CONDITION(mOffGroundOnly, mMode.mValue == Mode::Spin);

        mNames = {
            {Lowercase, "derp"},
            {LowercaseSpaced, "derp"},
            {Normal, "Derp"},
            {NormalSpaced, "Derp"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
    void onPacketOutEvent2(class PacketOutEvent& event);
};