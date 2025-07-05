//
// Created by vastrakai on 4/19/2024.
//


#pragma once

#include <Prax/Module/Module.h>

class AntiCheatDetector : public ModuleBase<AntiCheatDetector> {
public:
    enum class AntiCheats {
        None,
        FlareonV1,
        FlareonV2
    };

    BoolSetting V2NotifyOnly = BoolSetting("V2 Notify Only", "Only notify when Flareon V2 is detected", false);

    AntiCheatDetector() : ModuleBase("AntiCheatDetector", "Detects the Anti-Cheat that you're currently playing on", "Misc", 0, false)
    {
        AddSetting(&V2NotifyOnly);
    }

    static uint64_t LastPingPacket;
    static AntiCheats DetectedAntiCheat;
    static uint64_t LastCheck;

    void OnTick() override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;
    void OnEnable() override;
};