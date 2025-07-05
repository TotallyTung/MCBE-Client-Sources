//
// Created by vastrakai on 12/30/2023.
//


#pragma once
#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>


class DamageFly : public ModuleBase<DamageFly> {
public:
    NumberSetting Speed = NumberSetting("Speed", "How fast you go", 0.1f, 20.f, 10.f, 0.1f);
    NumberSetting Glide = NumberSetting("Glide", "How much you should glide", -2.f, 0.f, 0.f, 0.01);
    NumberSetting FlyMs = NumberSetting("Fly Ms", "How long you should fly for", 0.f, 5000.f, 100.f, 0.01);
    NumberSetting Ticks = NumberSetting("Ticks", "The amount of ticks before friction is reset", 1.f, 120.f, 20.f, 0.01);
    ModeSetting FrictionMode = ModeSetting("Friction Mode", "The mode for glide", {"OnReset", "Always"}, 1);
    NumberSetting GlideFrictionMultiplier = NumberSetting("Glide Friction Multiplier", "The multiplier for friction", 0, 2, 1.10f, 0.01);
    NumberSetting SpeedFrictionMultiplier = NumberSetting("Speed Friction Multiplier", "The multiplier for friction", 0, 2, 0.98f, 0.01);
    BoolSetting TimerBoostEnabled = BoolSetting("Timer Boost Enabled", "Whether or not to boost the timer", false);
    NumberSetting TimerBoost = NumberSetting("Timer Boost", "The timer to set when this module is enabled", 1.f, 120, 20.f, 0.01);
    BoolSetting ApplyJumpFlags = BoolSetting("Apply Jump Flags", "Applies jump flags, may or may not help with bypassing", false);

    DamageFly() : ModuleBase("DamageFly", "Fly after taking damage", "Movement", 0, false)
    {
        AddSetting(&Speed);
        AddSetting(&Glide);
        AddSetting(&FlyMs);
        AddSetting(&Ticks);
        AddSetting(&FrictionMode);
        AddSetting(&GlideFrictionMultiplier);
        AddSetting(&SpeedFrictionMultiplier);
        AddSetting(&TimerBoostEnabled);
        AddSetting(&TimerBoost);
        AddSetting(&ApplyJumpFlags);
    }

    static uintptr_t LastDamageTime;
    static uintptr_t Tick;
    static bool WasTimerBoosted;

    void ResetTimer();
    void OnTick() override;
    void OnDisable() override;
    void OnEnable() override;
    void OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled) override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) override;
};
