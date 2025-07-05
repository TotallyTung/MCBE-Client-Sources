#pragma once
//
// Created by vastrakai on 9/5/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>

class Ambience : public ModuleBase<Ambience> {
public:
    BoolSetting ChangeTime = BoolSetting("Change Time", "Changes the time", false);
    NumberSetting TimeValue = NumberSetting("Time Value", "The time value to set", 0, 24000, 18000, 1, &ChangeTime);
    ModeSetting Weather = ModeSetting("Weather", "The weather to set", {"Do not override", "Clear", "Rain"}, 0);
    BoolSetting RainbowSky = BoolSetting("Rainbow Sky", "Makes the sky rainbow", false);

    Ambience() : ModuleBase("Ambience", "Changes the ambience", "World", 0, false) {
        AddSetting(&ChangeTime);
        AddSetting(&TimeValue);
        AddSetting(&Weather);
        AddSetting(&RainbowSky);
    };

    void OnEnable() override;
    void OnDisable() override;
    void OnInitialize() override;
    void OnTick() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;
    void OnRender() override;
};