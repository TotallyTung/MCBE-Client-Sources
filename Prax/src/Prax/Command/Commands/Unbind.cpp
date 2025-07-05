//
// Created by Flash on 7/30/2023.
//

#include <src/Prax/SDK/Game/Minecraft.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Utils/System/KeyManager.h>
#include <Prax/Utils/String.h>
#include "Unbind.h"

void Unbind::OnRun(std::vector<std::string> args) {

    if (args.size() < 1) {
        AddCommandMessage("Usage: .unbind <module>");
        return;
    }

    std::string moduleName = args[0];

    for (auto mod : ModuleManager::Modules){
        if (String::ToLower(moduleName) == "all") {
            mod->KeyId = 0;
            continue;
        }

        if (String::ToLower(mod->Name) == String::ToLower(moduleName)) {

            mod->KeyId = 0;

            AddCommandMessage("Successfully unbound '" + moduleName + "'");
            return;
        }
    }

    if (String::ToLower(moduleName) == "all") {
        AddCommandMessage("Successfully unbound all modules!");
        return;
    }
    AddCommandMessage("Failed to find module by name '" + moduleName + "'");
}
