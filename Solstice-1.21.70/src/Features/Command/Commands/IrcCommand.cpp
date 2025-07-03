//
// Created by vastrakai on 8/24/2024.
//

#include "IrcCommand.hpp"

#include <Features/IRC/IrcClient.hpp>

void IrcCommand::execute(const std::vector<std::string>& args)
{
    if (!IrcManager::isConnected())
    {
        ChatUtils::displayClientMessage("§cYou are not connected to IRC.");
        return;
    }

    if (args.size() < 2)
    {
        ChatUtils::displayClientMessage("§c" + getUsage());
        return;
    }

    if (args[1] == "list")
    {
        // The server will send back the response message itself,
        // no need to display anything here
        IrcManager::requestListUsers();
    }
    else if (args[1] == "name")
    {
        if (args.size() < 3)
        {
            ChatUtils::displayClientMessage("§c" + getUsage());
            return;
        }

        IrcManager::requestChangeUsername(args[2]);
    }
    else
    {
        ChatUtils::displayClientMessage("§c" + getUsage());
    }
}

std::vector<std::string> IrcCommand::getAliases() const
{
    return {};
}

std::string IrcCommand::getDescription() const
{
    return "Interact with the IRC client";
}

std::string IrcCommand::getUsage() const
{
    return "Usage: .irc <list|name> [name]";
}
