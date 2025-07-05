#pragma once
//
// Created by vastrakai on 8/27/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class LongJump : public ModuleBase<LongJump> {
public:
    NumberSetting Speed = NumberSetting("Speed", "How fast you go", 0, 40, 1, 0.1);
    NumberSetting Height = NumberSetting("Height", "How high you go", 0, 10, 1, 0.1);
    NumberSetting Timer = NumberSetting("Timer", "The timer to set when this module is enabled\nSet to -0.1 to not override timer", -0.1, 120, 28.30f, 0.01);
    BoolSetting DisableModules = BoolSetting("Disable Modules", "Disables other modules bound to the same key", false);

    LongJump() : ModuleBase("LongJump", "Lets you jump long distances", "Movement", 0, false) {
        Speed.Display = true;
        AddSetting(&Speed);
        AddSetting(&Height);
        AddSetting(&Timer);
        AddSetting(&DisableModules);
    }

    static bool hasJumped;

    void OnDisable() override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
    void OnTick() override;
};