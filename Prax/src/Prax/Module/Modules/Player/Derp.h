#pragma once
//
// Created by vastrakai on 9/17/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>

class Derp : public ModuleBase<Derp> {
public:
    enum Mode {
        SPIN,
        RANDOM,
        HEADROLL,
        NONE,
        SCHIZO,
        STATIC
    };

    ModeSetting Mode = ModeSetting("Mode", "The mode to use for Derp", {"Spin", "Random", "HeadRoll", "None", "Schizo", "Static"}, 0);
    NumberSetting Speed = NumberSetting("Speed", "The speed of the Derp", 0, 5, 1, 0.1);
    BoolSetting HeadYaw = BoolSetting("HeadYaw", "Plays with your head yaw and makes derp look goofy :) (you can NOT see this clientside)", false);
    BoolSetting Flip = BoolSetting("Flip", "Flips your head", false);
    NumberSetting FlipTicks = NumberSetting("Flip Ticks", "The amount of ticks to flip your head", 0, 20, 10, 1);
    BoolSetting Nod = BoolSetting("Nod", "Makes your head nod", false);
    NumberSetting NodValue = NumberSetting("Nod Value", "The value of the nod", 0, 0.4, 0.09, 0.01);

    Derp() : ModuleBase("Derp", "Plays with your rotations.\nAny other modules that set rotations will override this.", "Player", 0, false) {
        Mode.Display = true;
        PacketPriority = Priority::AbsoluteFirst;
        AddSetting(&Mode);
        AddSetting(&Speed);
        AddSetting(&HeadYaw);
        AddSetting(&Flip);
        AddSetting(&FlipTicks);
        AddSetting(&Nod);
        AddSetting(&NodValue);
    }

    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
};