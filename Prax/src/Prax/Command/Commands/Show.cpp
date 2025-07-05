//
// Created by Flash on 1/2/2024.
//

#include "Show.h"

#include <Prax/Module/ModuleManager.h>

void Show::OnRun(std::vector<std::string> args) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (args.size() < 1) {
        ShowUsage();
        return;
    }

    std::string moduleName = String::ToLower(args[0]);

    if (moduleName == "all") {
        for (auto module : ModuleManager::Modules) {
            module->HideInArraylist = false;
        }
        Logger::Write("Command", "All modules are now shown in the arraylist", Logger::LogType::Debug);
        return;
    }


    if (moduleName == "none") {
        for (auto module : ModuleManager::Modules) {
            module->HideInArraylist = false;
        }
        Logger::Write("Command", "All modules are now shown in the arraylist", Logger::LogType::Debug);
        return;
    }

    Module* module = ModuleManager::FindModuleByName(moduleName);
    if (module == nullptr) {
        Logger::Write("Command", "Module not found!", Logger::LogType::Error);
        return;
    }

    module->HideInArraylist = false;
    Logger::Write("Command", "Module " + module->Name + " is now shown in the arraylist", Logger::LogType::Debug);
}
