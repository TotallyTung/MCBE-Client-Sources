//
// Created by vastrakai on 1/27/2024.
//


#pragma once
#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>

class KickSounds : public ModuleBase<KickSounds> {
public:
    enum KickSound {
        Fart
    };

    ModeSetting Mode = ModeSetting("Sound", "The sound to play. You can change these sounds in the Prax assets folder.", {"Fart"}, 0);
    NumberSetting Volume = NumberSetting("Volume", "The volume of the sound", 0, 1, 0.2f, 0.1f);

    KickSounds() : ModuleBase("KickSounds", "Plays a sound when you get kicked from a server", "Misc", 0, false) {
        AddSetting(&Mode);
        AddSetting(&Volume);
        Mode.Display = true;
    }

    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) override;
};
