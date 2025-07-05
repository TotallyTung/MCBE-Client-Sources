#pragma once
//
// Created by vastrakai on 9/15/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/BoolSetting.h>

class AutoQueue : public ModuleBase<AutoQueue> {
public:
    NumberSetting QueueDelay = NumberSetting("Queue Delay", "The delay between queueing for games (in milliseconds)", 0, 5000, 0, 100);
    BoolSetting ShowInChat = BoolSetting("Show In Chat", "Show the current game in chat", true);
    BoolSetting QueueOnDeath = BoolSetting("Queue On Death", "Queue for a game when you die", true);
    BoolSetting QueueOnGameEnd = BoolSetting("Queue On Game End", "Queue for a game when the game ends", true);

    AutoQueue() : ModuleBase("AutoQueue", "Automatically queues for games on The Hive", "Misc", 0, false){
        AddSetting(&QueueDelay);
        AddSetting(&ShowInChat);
        AddSetting(&QueueOnDeath);
        AddSetting(&QueueOnGameEnd);
    }

    static bool queueForGame;
    static std::string LastGame;
    static uint64_t lastQueueTime;

    static void QueueForGame();
    void OnTick() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;
    void OnEnable() override;


};