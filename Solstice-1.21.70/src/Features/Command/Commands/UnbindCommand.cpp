//
// Created by vastrakai on 6/29/2024.
//

#include "UnbindCommand.hpp"

#include <Utils/GameUtils/ChatUtils.hpp>

void UnbindCommand::execute(const std::vector<std::string>& args)
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

    if (module->mKey == 0)
    {
        ChatUtils::displayClientMessage("§cThe module §6'" + moduleName + "'§c is already unbound!");
        return;
    }

    module->mKey = 0;

    ChatUtils::displayClientMessage("§aModule §6'" + moduleName + "'§a has been unbound!");

}

std::vector<std::string> UnbindCommand::getAliases() const
{
    return {"u", "ub"};
}

std::string UnbindCommand::getDescription() const
{
    return "Unbinds the specified module.";
}

std::string UnbindCommand::getUsage() const
{
    return "Usage: .unbind <module>";
}
