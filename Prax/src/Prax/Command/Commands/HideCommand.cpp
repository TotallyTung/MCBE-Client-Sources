//
// Created by vastrakai on 5/1/2024.
//

#include "HideCommand.h"

#include <Prax/Module/ModuleManager.h>

void HideCommand::OnRun(std::vector<std::string> args)
{
    if (args.size() < 1) {
        ShowUsage();
        return;
    }

    std::string moduleName = args[0];

    if (moduleName == "unbound")
    {
        // Hide unbound modules
        std::vector<Module*> unboundModules;
        for (auto module : ModuleManager::Modules)
        {
            if (module->KeyId == 0)
            {
                module->HideInArraylist = true;
                unboundModules.push_back(module);
            }
        }

        AddCommandMessage("&7Hiding &f" + std::to_string(unboundModules.size()) + " &7unbound modules!");
        return;
    }

    Module* module = ModuleManager::FindModuleByName(moduleName);

    if (module == nullptr)
    {
        AddCommandMessage("&cThe module &f" + moduleName + " &cwas not found!");
        return;
    }

    bool hidden = !module->HideInArraylist;
    module->HideInArraylist = hidden;

    AddCommandMessage("&f" + module->Name + " &7is now " + (hidden ? "&chidden" : "&ashown") + "&7 in the arraylist!");
}
