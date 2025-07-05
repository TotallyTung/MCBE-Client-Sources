#pragma once
//
// Created by vastrakai on 9/17/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/BoolSetting.h>

class ParticleSpam : public ModuleBase<ParticleSpam> {
public:
    NumberSetting Intensity = NumberSetting("Intensity", "The intensity of the particles", 0, 40, 5, 1);
    BoolSetting Delay = BoolSetting("Delay", "Delays bursts", false);
    NumberSetting DelayTime = NumberSetting("Delay Time", "Delay time between bursts (in milliseconds)", 0, 10000, 0, 1);
    NumberSetting BurstTime = NumberSetting("Burst Time", "Burst time (in milliseconds)", 0, 10000, 100, 1);

    ParticleSpam() : ModuleBase("ParticleSpam", "Spams particles to make people lag", "Player", 0, false)
    {
        AddSetting(&Intensity);
        AddSetting(&Delay);
        AddSetting(&DelayTime);
        AddSetting(&BurstTime);
    };


    static uint64_t LastBurst;
    static uint64_t LastDelay;
    static bool Bursting;

    void OnTick() override;
};