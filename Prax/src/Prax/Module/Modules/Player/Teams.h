#pragma once
//
// Created by vastrakai on 9/7/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>

class Teams : public ModuleBase<Teams> {
public:
    ModeSetting Mode = ModeSetting("Mode", "The mode to use for teams", {"Hive"}, 0);
    static bool TeamsEnabled;

    Teams() : ModuleBase("Teams", "Ignores players that are on your team", "Player", 0, false) {
        Mode.Display = true;
        AddSetting(&Mode);
    }

    static bool IsOnTeam(Actor* actor);

    void OnEnable() override;
    void OnDisable() override;
};