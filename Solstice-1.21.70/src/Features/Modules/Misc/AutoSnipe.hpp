//
// Created by alteik on 03/09/2024.
//
#include <Features/Modules/Module.hpp>
#pragma once

class AutoSnipe : public ModuleBase<AutoSnipe> {
public:

    BoolSetting mQueueWhenTargetsLost = BoolSetting("Queue When Targets Lost", "Queue for a game when all targets are lost", true);
    BoolSetting mQueueWhenNoTargetsFound = BoolSetting("Queue When No Targets Found", "Queue for a game when no targets are found and voting has ended", true);
    BoolSetting mQueueWhenFull = BoolSetting("Queue When Full", "Queue for a game when the game is full and no targets are found", true);
    BoolSetting mRetryQueue = BoolSetting("Retry Queue", "Retry queueing for a game if the queue fails", true);

    AutoSnipe() : ModuleBase("AutoSnipe", "Snipes added targets", ModuleCategory::Misc, 0, false){
        addSettings( &mQueueWhenTargetsLost, &mQueueWhenNoTargetsFound, &mQueueWhenFull, &mRetryQueue);

        mNames = {
                {NamingStyle::Lowercase, "autosnipe"},
                {NamingStyle::LowercaseSpaced, "auto snipe"},
                {NamingStyle::Normal, "AutoSnipe"},
                {NamingStyle::NormalSpaced, "Auto Snipe"}
        };
    }

    static std::vector<std::string> Targets;

    void onEnable() override;
    void onDisable() override;
    bool AnyTargetsFound();
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);

};
