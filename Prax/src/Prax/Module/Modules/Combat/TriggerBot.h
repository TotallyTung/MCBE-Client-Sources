//
// Created by vastrakai on 1/27/2024.
//


#pragma once
#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class TriggerBot : public ModuleBase<TriggerBot> {
public:
    NumberSetting MinAps = NumberSetting("Min Aps", "Minimum attack speed", 1, 20, 20, 1);
    NumberSetting MaxAps = NumberSetting("Max Aps", "Maximum attack speed", 1, 20, 20, 1);
    BoolSetting HoldLeftClick = BoolSetting("Hold Left Click", "Only attacks when left click is held", false);
    BoolSetting UseAntibot = BoolSetting("Use Antibot", "Only attacks players (according to antibot settings)", false);

    TriggerBot() : ModuleBase("TriggerBot", "Automatically attacks when you look at a player", "Combat", 0, false) {
        AddSetting(&MinAps);
        AddSetting(&MaxAps);
        AddSetting(&HoldLeftClick);
        AddSetting(&UseAntibot);
    }

    Actor* getActorFromEntityId(EntityId entityId);
    void OnTick() override;
    void OnEnable() override;
    void OnDisable() override;
};
