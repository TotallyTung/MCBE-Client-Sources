#pragma once

#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Utils/Math.h>
#include "../../Module.h"

class OreESP : public ModuleBase<OreESP> {
public:

    NumberSetting DistanceSetting = NumberSetting("Distance", "The maximum distance to show blocks", 0, 20, 10.0f, 0.01);
    BoolSetting RedstoneSetting = BoolSetting("Redstone", "Whether or not to render redstone", true);
    BoolSetting DiamondSetting = BoolSetting("Diamond", "Whether or not to render diamond", true);
    BoolSetting GoldSetting = BoolSetting("Gold", "Whether or not to render gold", false);
    BoolSetting IronSetting = BoolSetting("Iron", "Whether or not to render iron", false);
    BoolSetting LapisSetting = BoolSetting("Lapis", "Whether or not to render lapis", false);
    BoolSetting EmeraldSetting = BoolSetting("Emerald", "Whether or not to render emerald", true);
    BoolSetting CoalSetting = BoolSetting("Coal", "Whether or not to render coal", false);
    BoolSetting BarrierSetting = BoolSetting("Barrier", "Whether or not to render barrier", false);
    NumberSetting UpdateFrequency = NumberSetting("Update Frequency", "How often to update the blocks (Measured in ticks)", 1, 20, 1.0f, 0.01);

    OreESP() : ModuleBase("OreESP", "Shows the selected ores with a corresponding box", "Visual", 0, false) {
        DistanceSetting.Display = true;
        AddSetting(&DistanceSetting);
        AddSetting(&RedstoneSetting);
        AddSetting(&DiamondSetting);
        AddSetting(&GoldSetting);
        AddSetting(&IronSetting);
        AddSetting(&LapisSetting);
        AddSetting(&EmeraldSetting);
        AddSetting(&CoalSetting);
        AddSetting(&BarrierSetting);
        AddSetting(&UpdateFrequency);

    }

    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;
    void OnGetBlock(Vector3& blockPos, Block* block);
    void DrawBoxAroundBlock(Vector3 blockPos, int lol);
    void OnRender() override;


};