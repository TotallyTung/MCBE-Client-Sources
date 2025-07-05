//
// Created by Flash on 7/30/2023.
//

#include <src/Prax/SDK/Game/Minecraft.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Utils/System/KeyManager.h>
#include <Prax/Utils/String.h>
#include "Binds.h"

void Binds::OnRun(std::vector<std::string> args) {
    // List all binds
    if (args.size() < 1) {
        std::string message = "Binds: \n";

        for (auto mod: ModuleManager::Modules) {
            if (mod->KeyId != 0) {
                // Get the key name
                auto keys = KeyManager::KeyMap;

                for (auto key: keys) {
                    if (key.second == mod->KeyId) {
                        message += mod->Name + " - " + key.first + "\n";
                    }
                }
            }
        }

        AddCommandMessage(message);
        return;
    }
}