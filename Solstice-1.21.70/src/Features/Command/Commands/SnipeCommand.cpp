//
// Created by alteik on 03/09/2024.
//

#include "SnipeCommand.hpp"
#include <Features/Modules/Misc/AutoSnipe.hpp>
#include <Utils/GameUtils/ChatUtils.hpp>
#include <Utils/MiscUtils/ColorUtils.hpp>

void SnipeCommand::execute(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        ChatUtils::displayClientMessage(getUsage());
        return;
    }

    auto autoSnipe = gFeatureManager->mModuleManager->getModule<AutoSnipe>();
    if (autoSnipe == nullptr)
    {
        ChatUtils::displayClientMessage("[AutoSnipe] ERROR: AutoSnipe module not found.");
        return;
    }

    if (!autoSnipe->mEnabled)
    {
        ChatUtils::displayClientMessage("[AutoSnipe] AutoSnipe is not enabled!");
        return;
    }

    const std::string& subCommand = args[1];

    if(subCommand == "add")
    {
        const std::string& nickName = args[2];
        autoSnipe->Targets.emplace_back(nickName);
        ChatUtils::displayClientMessage("Added target: " + nickName);
    }
    else if(subCommand == "remove")
    {
        const std::string& nickName = args[2];
        for (auto it = autoSnipe->Targets.begin(); it != autoSnipe->Targets.end(); ) {
            if (*it == nickName) {
                it = autoSnipe->Targets.erase(it);
                ChatUtils::displayClientMessage("Removed target: " + nickName);
                return;
            } else {
                ++it;
            }
        }
        ChatUtils::displayClientMessage("Target: " + nickName + " not found");
    }
    else if(subCommand == "clear")
    {
        autoSnipe->Targets.clear();
        ChatUtils::displayClientMessage("Successfuly cleaned targets list");
    }
    else
    {
        ChatUtils::displayClientMessage(getUsage());
        return;
    }
}

std::vector<std::string> SnipeCommand::getAliases() const
{
    return {"tg"};
}

std::string SnipeCommand::getDescription() const
{
    return "Add or remove a target to the AutoSnipe module.";
}

std::string SnipeCommand::getUsage() const
{
    return "Usage .snipe add/remove";
}
