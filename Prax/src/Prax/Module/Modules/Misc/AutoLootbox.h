#pragma once
//
// Created by vastrakai on 9/23/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>

class AutoLootbox : public ModuleBase<AutoLootbox> {
public:
    enum class Mode {
        Break,
        Open
    };
    ModeSetting Mode = ModeSetting("Mode", "The mode of the module", { "Break", "Open" }, (int)Mode::Break);

    AutoLootbox() : ModuleBase("AutoLootbox", "Automatically breaks lootboxes on The Hive", "Misc", 0, false)
    {
        AddSetting(&Mode);
    }

    static Vector2 Rotation;
    static uint64_t LastHitMs;

    void RotateToEntity(Actor *actor);
    void OnTick() override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
};
