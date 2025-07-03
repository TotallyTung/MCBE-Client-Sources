//
// Created by alteik on 04/10/2024.
//

#include <Features/Modules/Module.hpp>
#include <Features/Modules/Setting.hpp>
#pragma once

class AirSpeed : public ModuleBase<AirSpeed> {
public:

    NumberSetting mSprintSpeed = NumberSetting("Speed with sprint", "speed in air while sprinting", 1.0f, 1.f, 5.0f, 0.01f);
    NumberSetting mNoSprintSpeed = NumberSetting("Speed without sprint", "speed in air while not sprinting", 1.0f, 1.f, 5.0f, 0.01f);

    AirSpeed() : ModuleBase("AirSpeed", "Changes speed in air", ModuleCategory::Movement, 0, false) {
        addSettings(&mSprintSpeed, &mNoSprintSpeed);

        mNames = {
                {Lowercase, "airspeed"},
                {LowercaseSpaced, "air speed"},
                {Normal, "AirSpeed"},
                {NormalSpaced, "Air Speed"}
        };
    }

    float* mSpeedInAirWithSprintPtr;
    float* mSpeedInAirWithoutSprintPtr;
    float mSpeedInAirWithSprintValue = 0.026f;
    float mSpeedInAirWithoutSprintValue = 0.020f;

    void onEnable() override;
    void onDisable() override;
    void onInit() override;
    void onBaseTickEvent(class BaseTickEvent& event);
};