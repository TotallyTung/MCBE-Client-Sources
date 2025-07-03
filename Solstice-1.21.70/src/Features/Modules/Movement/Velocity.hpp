#pragma once
#include <Features/Modules/Module.hpp>
//
// Created by vastrakai on 7/2/2024.
//


class Velocity : public ModuleBase<Velocity> {
public:
    enum class Mode {
        Full,
        Percent,
    };
    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode of the velocity.", Mode::Full, "Full", "Percent");
    NumberSetting mHorizontal = NumberSetting("Horizontal", "The multiplier for the horizontal velocity", 0, -2, 2, 0.01);
    NumberSetting mVertical = NumberSetting("Vertical", "The multiplier for the vertical velocity", 0, -2, 2, 0.01);


    Velocity() : ModuleBase("Velocity", "Allows you to modify your velocity from attacks, explosions, etc.", ModuleCategory::Movement, 0, false) {
        addSettings(&mMode, &mHorizontal, &mVertical);
        VISIBILITY_CONDITION(mHorizontal, mMode.mValue == Mode::Percent);
        VISIBILITY_CONDITION(mVertical, mMode.mValue == Mode::Percent);

        mNames = {
            {Lowercase, "velocity"},
            {LowercaseSpaced, "velocity"},
            {Normal, "Velocity"},
            {NormalSpaced, "Velocity"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onPacketInEvent(class PacketInEvent& event) const;

    std::string getSettingDisplay() override
    {
        return mMode.mValue == Mode::Full ? "Full" : fmt::format("{}% {}%", static_cast<int>(mHorizontal.mValue * 100), static_cast<int>(mVertical.mValue * 100));
    }
};