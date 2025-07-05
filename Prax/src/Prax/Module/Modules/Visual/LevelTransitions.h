//
// Created by vastrakai on 4/19/2024.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class LevelTransitions : public ModuleBase<LevelTransitions> {
public:
    NumberSetting TransitionSpeed = NumberSetting("Transition Speed", "The speed of the transition", 0.01f, 5.f, 1.f, 0.01f);

    LevelTransitions() : ModuleBase("LevelTransitions", "Shows the level transitions", "Visual") {
        AddSetting(&TransitionSpeed);
    }

    static uint64_t LastDimensionChange;
    static float AnimPerc;
    static bool IsTransitioning;

    void OnRender() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled);
};