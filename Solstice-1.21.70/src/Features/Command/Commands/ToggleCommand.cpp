//
// Created by vastrakai on 6/29/2024.
//

#include "ToggleCommand.hpp"

#include <Utils/GameUtils/ChatUtils.hpp>

void ToggleCommand::execute(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        ChatUtils::displayClientMessage("§c" + getUsage());
        return;
    }

    std::string moduleName = args[1];
    Module* module = gFeatureManager->mModuleManager->getModule(moduleName);

    if (module == nullptr)
    {
        ChatUtils::displayClientMessage("§cThe module §6'" + moduleName + "'§c does not exist!");
        return;
    }

    moduleName = module->mName;

    module->toggle();
    bool newState = !module->mEnabled; // The mEnabled isn't instantly updated
    ChatUtils::displayClientMessage("§aModule §6'" + moduleName + "'§a is now " + (newState ? "§aenabled" : "§cdisabled") + "§a!");



}

std::vector<std::string> ToggleCommand::getAliases() const
{
    return {"t"};
}

std::string ToggleCommand::getDescription() const
{
    return "Toggles the specified module";
}

std::string ToggleCommand::getUsage() const
{
    return "Usage: .toggle <module>";
}
