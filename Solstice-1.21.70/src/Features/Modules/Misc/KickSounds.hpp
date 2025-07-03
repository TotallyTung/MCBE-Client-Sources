#pragma once
//
// Created by vastrakai on 7/4/2024.
//

#include <Features/FeatureManager.hpp>


class KickSounds : public ModuleBase<KickSounds> {
public:
    enum Sound {
        Fard,
        Mario,
        Windows
    };

    EnumSettingT<Sound> mSound = EnumSettingT<Sound>("Sound", "The sound to play when a DisconnectPacket is received", Sound::Fard, "Fard", "Mario", "Windows");
    NumberSetting mVolume = NumberSetting("Volume", "The volume of the sound", 1.f, 0.f, 1.f, 0.1f);

    KickSounds() : ModuleBase("KickSounds", "Plays a sound when a DisconnectPacket is received", ModuleCategory::Misc, 0, false) {
        addSetting(&mSound);
        addSetting(&mVolume);

        mNames = {
            {Lowercase, "kicksounds"},
            {LowercaseSpaced, "kick sounds"},
            {Normal, "KickSounds"},
            {NormalSpaced, "Kick Sounds"}
        };
    }

    void onEnable() override;
    void onDisable() override;

    void onPacketInEvent(class PacketInEvent& event);

    std::string getSettingDisplay() override {
        return mSound.mValues[mSound.as<int>()];
    }

};