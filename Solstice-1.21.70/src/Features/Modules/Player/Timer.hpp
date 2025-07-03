#pragma once
//
// Created by vastrakai on 7/1/2024.
//

#include <Features/Modules/Module.hpp>
#include <Features/Modules/Setting.hpp>

class Timer : public ModuleBase<Timer> {
public:
    NumberSetting mSpeed = NumberSetting("Speed", "The speed of the game.", 20.0f, 1.f, 60.0f, 0.01f);
    Timer() : ModuleBase("Timer", "Changes the game tick speed", ModuleCategory::Player, VK_F4, false) {
        addSetting(&mSpeed);

        mNames = {
            {Lowercase, "timer"},
            {LowercaseSpaced, "timer"},
            {Normal, "Timer"},
            {NormalSpaced, "Timer"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);

    std::string getSettingDisplay() override {
        // Only display with 1 decimal place
        return fmt::format("{:.1f}", mSpeed.mValue);
    }
};