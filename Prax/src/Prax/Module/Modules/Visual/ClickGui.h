#pragma once

#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include "../../Module.h"

class ClickGuiMod : public ModuleBase<ClickGuiMod> {
public:

    enum class SortingModeE {
        None = -1,
        Alphabetical = 0,
        Length
    };

    enum class ClickGuiModeE {
        None = -1,
        New = 0,
        Old
    };

    enum class SettingDisplayModeE {
        None = -1,
        Integrated = 0,
        Seperate
    };

    static inline ModeSetting SortingMode = ModeSetting("Sorting", "The sorting mode of the ClickGui", {"Alphabetical", "Length"}, (int)SortingModeE::Length);
    static inline ModeSetting ClickGuiMode = ModeSetting("ClickGuiMode", "The mode of the ClickGui", {"New", "Old"}, 1);
    static inline ModeSetting SettingDisplayMode = ModeSetting("SettingDisplayMode", "The mode of the setting display", {"Integrated", "Seperate"}, (int)SettingDisplayModeE::Integrated);

    ClickGuiMod() : ModuleBase("ClickGui", "The ClickGui", "Visual", VK_INSERT, false) {
        AddSetting(&SortingMode);
        AddSetting(&ClickGuiMode);
        AddSetting(&SettingDisplayMode);
        RenderOnDisabled = true;
    }

    void OnEnable() override;
    void OnRender() override;
    void OnDisable() override;
    void OnTick() override;
};