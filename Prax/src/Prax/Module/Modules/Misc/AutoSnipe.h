//
// Created by vastrakai on 4/13/2024.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class AutoSnipe : public ModuleBase<AutoSnipe> {
public:
    BoolSetting TargetNotifications = BoolSetting("Target Notifications", "Show notifications when a target is found, or lost", true);
    BoolSetting QueueWhenTargetsLost = BoolSetting("Queue When Targets Lost", "Queue for a game when all targets are lost", true);
    BoolSetting QueueWhenNoTargetsFound = BoolSetting("Queue When No Targets Found", "Queue for a game when no targets are found and voting has ended", true);
    BoolSetting QueueWhenFull = BoolSetting("Queue When Full", "Queue for a game when the game is full and no targets are found", true);
    BoolSetting RetryQueue = BoolSetting("Retry Queue", "Retry queueing for a game if the queue fails", true);
    NumberSetting Volume = NumberSetting("Volume", "Volume of the notification sound", 0, 1.f, 0.75f, 1);

    AutoSnipe() : ModuleBase("AutoSnipe", "Automatically snipes players", "Misc", 0, false) {
        AddSetting(&TargetNotifications);
        AddSetting(&QueueWhenTargetsLost);
        AddSetting(&QueueWhenNoTargetsFound);
        AddSetting(&QueueWhenFull);
        AddSetting(&RetryQueue);
        AddSetting(&Volume);
    }

    static std::vector<std::string> Targets;

    void SendNotification(std::string message);
    bool AnyTargetsFound();

    void OnTick() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;
    void OnEnable() override;
    void OnInitialize() override;

};