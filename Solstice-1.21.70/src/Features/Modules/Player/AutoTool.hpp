#pragma once
//
// Created by vastrakai on 7/12/2024.
//

#include <Features/Modules/Module.hpp>

class AutoTool : public ModuleBase<AutoTool> {
public:
    BoolSetting mHotbarOnly = BoolSetting{ "Hotbar Only", "Only switch to tools in the hotbar", false };
    BoolSetting mFakeSpoof = BoolSetting{ "Fake Spoof", "Spoof the tool in the hotbar", false };
    AutoTool() : ModuleBase("AutoTool", "Automatically switches to the fastest tool for the block you're mining", ModuleCategory::Player, 0, false)
    {
        addSetting(&mHotbarOnly);
        addSetting(&mFakeSpoof);

        mNames = {
              {Lowercase, "autotool"},
                {LowercaseSpaced, "auto tool"},
                {Normal, "AutoTool"},
                {NormalSpaced, "Auto Tool"}
        };
    };

    int mOldSlot = -1;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
};