//
// Created by vastrakai on 10/28/2023.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class AntiVoid : public ModuleBase<AntiVoid> {
public:
    NumberSetting Distance = NumberSetting("Distance", "The distance to fall before teleporting you back up", 0, 12, 4, 1);
    BoolSetting Teleport = BoolSetting("Teleport", "Teleport you back up", false);
    BoolSetting VoidCheck = BoolSetting("Void Check", "Check if you are in the void", false);
    BoolSetting EnableFreecam = BoolSetting("Enable Freecam", "Enable freecam when you fall", false);
    BoolSetting EnableScaffold = BoolSetting("Enable Scaffold", "Enable scaffold when you fall", false);
    BoolSetting DisableRegen = BoolSetting("Disable Regen", "Disable regen when you fall", false);
    BoolSetting TpOnce = BoolSetting("Tp Once", "Only attempts to teleport you once", false);

    AntiVoid() : ModuleBase("AntiVoid", "Prevents you from falling into the void", "Player", VK_NUMPAD0, false) {
        AddSetting(&Distance);
        AddSetting(&Teleport);
        AddSetting(&VoidCheck);
        AddSetting(&EnableFreecam);
        AddSetting(&EnableScaffold);
        AddSetting(&DisableRegen);
        AddSetting(&TpOnce);
    }

    static std::vector<Vector3> LastOnGroundPositions;

    void OnEnable();
    void OnDisable();
    void OnTick();


};