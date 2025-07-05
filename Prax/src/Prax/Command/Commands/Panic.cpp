//
// Created by vastrakai on 1/27/2024.
//

#include "Panic.h"

#include <Prax/Module/ModuleManager.h>

void Panic::OnRun(std::vector<std::string> args)
{
    // Disable all modules
    for (auto& module : ModuleManager::Modules)
    {
        if (module->Enabled) module->Toggle();
    }
}
