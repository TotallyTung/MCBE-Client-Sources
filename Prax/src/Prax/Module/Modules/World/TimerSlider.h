#pragma once
//
// Created by vastrakai on 9/4/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/BoolSetting.h>

class TimerSlider : public ModuleBase<TimerSlider> {
public:
    NumberSetting SpeedSetting = NumberSetting("Speed", "Speed", 0.01f, 100, 21.40f, 0.1);
    NumberSetting MinSpeedSetting = NumberSetting("Min Speed", "Min Speed", 0.01f, 100, 25.00f, 0.1);
    NumberSetting AttribShift = NumberSetting("Time Attribute Shift", "How much to shift the speed attribute by", -10.f, 10.f, -0.5f, 0.01f);
    NumberSetting MaxDivisor = NumberSetting("Max Divisor", "Max Divisor", 0.01f, 2.f, 1.7f, 0.01f);
    BoolSetting AdjustForSpeedAttributeSetting = BoolSetting("Adjust for Time Attribute", "Adjust for Time Attribute", true);
    BoolSetting DownMotion = BoolSetting("Down Motion", "Applies extra down velocity on a certain tick off-ground", false);
    NumberSetting DownMotionTick = NumberSetting("Down Motion Tick", "The tick to apply down motion on", 0, 20, 7, 1, &DownMotion);
    NumberSetting DownMotionPredict = NumberSetting("Down Motion Predict", "The amount of times to predict the next motion on that tick", 0, 20, 1, 1, &DownMotion);

    TimerSlider() : ModuleBase("TimerSlider", "Changes the game speed", "World", 0, false) {
        SpeedSetting.Display = true;
        AddSetting(&SpeedSetting);
        AddSetting(&MinSpeedSetting);
        AddSetting(&AttribShift);
        AddSetting(&MaxDivisor);
        AddSetting(&AdjustForSpeedAttributeSetting);
        AddSetting(&DownMotion);
        AddSetting(&DownMotionTick);
        AddSetting(&DownMotionPredict);
    };

    static int OffGroundTicks;


    float GetSpeedAttrib();

    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;


};