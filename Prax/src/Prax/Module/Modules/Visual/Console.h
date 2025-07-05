#pragma once

#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/ModuleManager.h>
#include "../../Module.h"

class ConsoleMod : public ModuleBase<ConsoleMod> {
public:

    ConsoleMod() : ModuleBase("Console", "AllocConsole but fancy", "Visual", VK_HOME, false) {
        RenderOnDisabled = true;
    }

    void OnEnable() override;
    void OnRender() override;
    void OnDisable() override;
    void OnTick() override;
};

//REGISTER_DEBUG_MODULE(ConsoleMod);