//
// Created by vastrakai on 7/10/2024.
//

#include "SetCommand.hpp"

#include <Features/FeatureManager.hpp>
#include <Utils/GameUtils/ChatUtils.hpp>

#include "spdlog/spdlog.h"

void SetCommand::execute(const std::vector<std::string>& args)
{
    if (args.size() < 4)
    {
        ChatUtils::displayClientMessage("§c" + getUsage());
        return;
    }

    std::string moduleName = args[1];
    std::string settingName = args[2];
    std::string value = args[3];

    auto module = gFeatureManager->mModuleManager->getModule(moduleName);
    if (!module)
    {
        ChatUtils::displayClientMessage("§cModule §6" + moduleName + "§c not found.");
        return;
    }

    auto setting = module->getSetting(settingName);
    if (!setting)
    {
        ChatUtils::displayClientMessage("§cSetting §6" + settingName + "§c not found.");
        return;
    }

    if (!setting->parse(value))
    {
        ChatUtils::displayClientMessage("§cInvalid value §6" + value + "§c for setting §6" + settingName + "§c.");
        return;
    }

    ChatUtils::displayClientMessage("§aSet §6" + setting->mName + "§a to §6" + value + "§a for module §6" + module->mName + "§a.");
}

std::vector<std::string> SetCommand::getAliases() const
{
    return { "s" };
}

std::string SetCommand::getDescription() const
{
    return "Sets a module setting to a value";
}

std::string SetCommand::getUsage() const
{
    return "Usage: .set <module> <setting> <value>";
}
