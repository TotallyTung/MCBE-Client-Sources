//
// Created by Flash on 7/30/2023.
//

#include <src/Prax/SDK/Game/Minecraft.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Utils/System/KeyManager.h>
#include <Prax/Utils/String.h>
#include "Bind.h"

void Bind::OnRun(std::vector<std::string> args) {

    if (args.size() < 1) {
        AddCommandMessage("Usage: .bind <module> <key>");
        return;
    }

    std::string moduleName = args[0];
    std::string key = String::ToUpper(args[1]);

    for (auto mod: ModuleManager::Modules) {
        if (String::ToLower(moduleName) == "all") {
            mod->KeyId = 0;
            continue;
        }

        if (String::ToLower(mod->Name) == String::ToLower(moduleName)) {
            if (String::ToLower(key) == "none") {
                mod->KeyId = 0;
                AddCommandMessage("Successfully unbound '" + moduleName + "'");
                return;
            } else {
                auto keys = KeyManager::KeyMap;
                auto it = keys.find(String::ToLower(key));

                if (it == keys.end()) {
                    AddCommandMessage("Failed to find key '" + key + "'");
                    return;
                }

                mod->KeyId = (int)it->second;

                AddCommandMessage("Successfully bound '" + moduleName + "' to '" + key + "'");
                return;

            }
        }
    }

    AddCommandMessage("Failed to find module by name '" + moduleName + "'");
}