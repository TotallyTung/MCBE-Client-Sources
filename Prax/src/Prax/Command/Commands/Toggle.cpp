//
// Created by Flash on 7/30/2023.
//

#pragma once


#include <Prax/Module/ModuleManager.h>
#include "Toggle.h"

void Toggle::OnRun(std::vector<std::string> args) {
    if (args.size() < 1) {
        AddCommandMessage("Usage: toggle <module>");
        return;
    }

    std::string moduleName = args[0];

    auto module = ModuleManager::FindModuleByName(moduleName);

    if (!module) {
        AddCommandMessage("Module not found");
        return;
    }

    module->Toggle();
    AddCommandMessage(module->Name + " has been " + (module->Enabled ? "enabled" : "disabled"));
}