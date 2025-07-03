#pragma once
//
// Created by vastrakai on 7/9/2024.
//

#include <Features/Modules/Module.hpp>

class AntiCheatDetector : public ModuleBase<AntiCheatDetector> {
public:
    enum class AntiCheats {
        None,
        FlareonV1,
        FlareonV2
    };

    BoolSetting V2NotifyOnly = BoolSetting("V2 Notify Only", "Only notify when Flareon V2 is detected", false);
    BoolSetting mPlaySound = BoolSetting("Play Sound", "Enable or disable sound notification", false);

    AntiCheatDetector() : ModuleBase("AntiCheatDetector", "Detects the Anti-Cheat that you're currently playing on", ModuleCategory::Misc, 0, false)
    {
        addSettings(&V2NotifyOnly);
        addSettings(&mPlaySound);

        mNames = {
            {Lowercase, "anticheatdetector"},
            {LowercaseSpaced, "anti cheat detector"},
            {Normal, "AntiCheatDetector"},
            {NormalSpaced, "Anti Cheat Detector"}
        };
    }

    uint64_t mLastPingPacket;
    AntiCheats mDetectedAntiCheat;
    uint64_t mLastCheck;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);


};