#pragma once
//
// Created by vastrakai on 7/19/2024.
//

#include <Features/Modules/Module.hpp>

class  Killsults : public ModuleBase<Killsults> {
public:
    enum class Mode {
        None,
        Custom
    };

    enum class KillSound {
        None,
        PopDing,
        Cash
    };

    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode of the killsults module", Mode::None, "None", "Custom");
    BoolSetting mAutoGG = BoolSetting("Auto GG", "Whether or not to send a message when you get a kill", false);
    EnumSettingT<KillSound> mKillSound = EnumSettingT<KillSound>("Kill Sound", "The sound to play when you get a kill", KillSound::None, "None", "Pop+Ding", "Cash");
    NumberSetting mSoundVolume = NumberSetting("Sound Volume", "The volume of the kill sound", 1.0f, 0.0f, 1.0f, 0.01f);
    BoolSetting mShowNotification = BoolSetting("Show Notification", "Whether or not to show a notification when you get a kill", true);

    Killsults() : ModuleBase("Killsults", "Automatically sends a chat message when you get a kill.", ModuleCategory::Misc, 0, false    )
    {
        addSettings(&mMode, &mAutoGG, &mKillSound, &mSoundVolume, &mShowNotification);

        VISIBILITY_CONDITION(mSoundVolume, mKillSound.mValue != KillSound::None);

        mNames = {
            {Lowercase, "killsults"},
            {LowercaseSpaced, "killsults"},
            {Normal, "Killsults"},
            {NormalSpaced, "Killsults"}
        };
    }
// MessageTemplate mVoteMessageTemplate = MessageTemplate("voteMessageTemplate", "Yo @here! Vote for !mapName!!!", std::make_pair("!mapName!", "The name of the map you voted for")); // example
    MessageTemplate mCustomKillsults = MessageTemplate("customKillsults", "Wow! I just killed !target! using my favorite client!",
        std::make_pair("!target!", "The player you killed"), std::make_pair("!localPlayer!", "Your username"), std::make_pair("!killCount!", "The number of kills you got this round (so far)"));
    MessageTemplate mAutoGGTemplate = MessageTemplate("autoGG", "GG. I got !killCount! kills this round.",
        std::make_pair("!killCount!", "The number of kills you got this round"), std::make_pair("!localPlayer!", "Your username"));

    std::map<int64_t, int64_t> mLastAttacked = {};
    std::map<int64_t, int64_t> mLastHurt = {};
    std::vector<std::string> mLastChatMessages = {};
    int mKillCount = 0;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);

    std::string getSettingDisplay() override
    {
        return mMode.mValues[mMode.as<int>()];
    }
};