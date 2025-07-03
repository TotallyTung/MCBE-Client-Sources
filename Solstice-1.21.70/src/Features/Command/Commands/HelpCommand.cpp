//
// Created by vastrakai on 6/29/2024.
//

#include "HelpCommand.hpp"

#include <Utils/GameUtils/ChatUtils.hpp>

void HelpCommand::execute(const std::vector<std::string>& args)
{
    const auto commands = gFeatureManager->mCommandManager->getCommands();

    if (args.size() > 1)
    {
        for (const auto& command : commands)
        {
            if (command->name == "fling") continue;
            if (command->name == args[1])
            {
                ChatUtils::displayClientMessage("§6Command: §e{}", command->name);
                ChatUtils::displayClientMessage("§6- §7{}", command->getDescription());
                ChatUtils::displayClientMessage("§6- §7{}", command->getUsage());
                if (!command->getAliases().empty())
                {
                    ChatUtils::displayClientMessage("§6- §7Aliases: §e{}", fmt::join(command->getAliases(), "§7, §e"));
                }
                return;
            }
        }

        ChatUtils::displayClientMessage("§cThe command §6'{}'§c does not exist!", args[1]);
        return;
    }

    ChatUtils::displayClientMessage("§6Commands:");

    for (const auto& command : commands)
    {
        ChatUtils::displayClientMessage("§6- §e{} §7- {}", command->name, command->getDescription());
    }

    int scriptCount = 0;
    bool first = true;
    for (const auto& s : gFeatureManager->mScriptManager->mScripts)
    {
        if (s->type != SolsticeScriptType::COMMAND) continue;

        auto command = std::static_pointer_cast<CommandScript>(s);

        if (first)
        {
            ChatUtils::displayClientMessage("§6Scripts:");
            first = false;
        }

        scriptCount++;
        ChatUtils::displayClientMessage("§6- §e{} §7- {}", command->commandName, command->commandDescription);
    }

    ChatUtils::displayClientMessage("§6Use §e.help <command> §6for more information on a specific command.");
}

std::vector<std::string> HelpCommand::getAliases() const
{
    return {"?"};
}

std::string HelpCommand::getDescription() const
{
    return "Displays a list of available commands";
}

std::string HelpCommand::getUsage() const
{
    return "Usage: .help";
}
