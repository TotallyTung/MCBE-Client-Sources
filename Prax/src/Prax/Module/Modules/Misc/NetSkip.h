#pragma once
//
// Created by vastrakai on 9/1/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/BoolSetting.h>

class NetSkip : public ModuleBase<NetSkip> {
public:
    ModeSetting Mode = ModeSetting("Mode", "The mode to use", {"Tick", "Delay"}, 0);
    NumberSetting Ticks = NumberSetting("Ticks", "How many Ticks to skip", 1, 200, 100, 1);
    NumberSetting Delay = NumberSetting("Delay", "How many ms to delay", 1, 20000, 100, 1);
    BoolSetting VisualizeSetting = BoolSetting("Visualize", "Visualize the current and last position", true);
    BoolSetting RenderCurrentAndLastPosSetting = BoolSetting("Render current pos", "Render the current and last position if enabled, \n"
                                                                                 "Otherwise, only render the last pos", false);
    BoolSetting DisableWhenScaffolding = BoolSetting("Disable When Scaffolding", "Disables when scaffolding", true);
    BoolSetting WeakEnable = BoolSetting("Weak Enable", "NaCl Bypass Developments. LLC.", false);

    NetSkip() : ModuleBase("NetSkip", "Skips network ticks", "Misc", 0, false) {
        Mode.Display = true;
        AddSetting(&Mode);
        AddSetting(&Ticks);
        AddSetting(&Delay);
        AddSetting(&VisualizeSetting);
        AddSetting(&RenderCurrentAndLastPosSetting);
        AddSetting(&DisableWhenScaffolding);
        AddSetting(&WeakEnable);
    };
    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;
};