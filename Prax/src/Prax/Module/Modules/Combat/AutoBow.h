#pragma once
//
// Created by vastrakai on 10/10/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class AutoBow : public ModuleBase<AutoBow> {
public:
    NumberSetting Ticks = NumberSetting("Ticks", "The delay between starting to use bow and releasing it", 0, 20, 15, 1);
    BoolSetting AuraOnly = BoolSetting("AuraOnly", "Only activates when you are in aura", true);
    NumberSetting MoveCompensation = NumberSetting("Move Compensation", "The multiplier for the move compensation", 0, 3, 1.f, 0.1);

    AutoBow() : ModuleBase("AutoBow", "Automatically shoots your bow", "Combat", 0, false) {
        PacketPriority = Priority::KindaLast;
        this->AddSetting(&Ticks);
        this->AddSetting(&AuraOnly);
        this->AddSetting(&MoveCompensation);
    };

    static bool RotateNextTick;

    void OnEnable() override;
    void OnDisable() override;
    void OnRender() override;
    void OnTick() override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel);
};