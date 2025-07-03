#pragma once
//
// Created by vastrakai on 7/5/2024.
//

#include <Features/Modules/Module.hpp>


class AutoQueue : public ModuleBase<AutoQueue> {
public:
    enum class Mode {
        Auto,
        Random
    };
    EnumSettingT<Mode> mMode = EnumSettingT("Mode", "The mode", Mode::Auto, "Auto", "Random");
    NumberSetting mQueueDelay = NumberSetting("Queue Delay", "The delay between queueing for games (in seconds)", 1.f, 0.f, 5.f, 0.01f);
    BoolSetting mShowInChat = BoolSetting("Show In Chat", "Show the current game in chat", true);
    BoolSetting mQueueOnDeath = BoolSetting("Queue On Death", "Queue for a game when you die", true);
    BoolSetting mQueueOnGameEnd = BoolSetting("Queue On Game End", "Queue for a game when the game ends", true);
    BoolSetting mFlyOnGameEnd = BoolSetting("Fly On Game End", "Enable fly module when the game ends", false);
    BoolSetting mDisableFly = BoolSetting("Disable Fly", "Disable fly module on dimension changes", false);

    AutoQueue() : ModuleBase("AutoQueue", "Automatically queues for games", ModuleCategory::Misc, 0, false){
        addSettings(&mMode, &mQueueDelay, &mShowInChat, &mQueueOnDeath, &mQueueOnGameEnd, &mFlyOnGameEnd, &mDisableFly);

        mNames = {
            {NamingStyle::Lowercase, "autoqueue"},
            {NamingStyle::LowercaseSpaced, "auto queue"},
            {NamingStyle::Normal, "AutoQueue"},
            {NamingStyle::NormalSpaced, "Auto Queue"}
        };
    }

    bool mQueueForGame = false;
    std::string mLastGame = "sky";
    uint64_t mLastQueueTime = 0;
    uint64_t mLastCommandExecuted = 0;
    std::map<uint64_t, std::string> mQueuedCommands = {};
    std::vector<std::string> mGamemodes = { "sky", "sky-duos", "sky-squads", "sky-mega", "ctf", "bed", "bed-duos", "bed-squads", "sg", "sg-duos" };

    void onEnable() override;
    void onDisable() override;
    void queueForGame();
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);

    std::string getSettingDisplay() override {
        return mMode.mValues[mMode.as<int>()];
    }
};
