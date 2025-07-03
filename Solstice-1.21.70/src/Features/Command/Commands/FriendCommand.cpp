//
// Created by vastrakai on 7/12/2024.
//

#include "FriendCommand.hpp"

#include <Features/Modules/Misc/Friends.hpp>
#include <Utils/GameUtils/ChatUtils.hpp>

void FriendCommand::execute(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        ChatUtils::displayClientMessage("§c" + getUsage());
        return;
    }

    const std::string& subCommand = args[1];
    auto friendMan = gFriendManager;

    if (subCommand == "add")
    {
        if (args.size() < 3)
        {
            ChatUtils::displayClientMessage("§c" + getUsage());
            return;
        }

        friendMan->addFriend(args[2]);
        ChatUtils::displayClientMessage("§aAdded " + args[2] + " to your friends list!");
    }
    else if (subCommand == "remove")
    {
        if (args.size() < 3)
        {
            ChatUtils::displayClientMessage("§c" + getUsage());
            return;
        }

        // check if the player is already in the friends list
        if (!friendMan->isFriend(args[2]))
        {
            ChatUtils::displayClientMessage("§c" + args[2] + " is not in your friends list!");
            return;
        }

        friendMan->removeFriend(args[2]);
        ChatUtils::displayClientMessage("§cRemoved " + args[2] + " from your friends list!");
    }
    else if (subCommand == "list")
    {
        if (friendMan->mFriends.empty())
        {
            ChatUtils::displayClientMessage("§cYou have no friends :(");
            return;
        }

        ChatUtils::displayClientMessage("§6Friends:");
        for (const auto& friendName : friendMan->mFriends)
        {
            ChatUtils::displayClientMessage("§7- " + friendName);
        }
        ChatUtils::displayClientMessage("§6Use §e.friend add/remove <name> §6to manage your friends list!");
    }
    else if (subCommand == "clear")
    {
        friendMan->clearFriends();
        ChatUtils::displayClientMessage("§aSuccessfully cleared your friends list!");
    }
    else
    {
        ChatUtils::displayClientMessage("§c" + getUsage());
    }

}

std::vector<std::string> FriendCommand::getAliases() const
{
    return { "f", "friends" };
}

std::string FriendCommand::getDescription() const
{
    return "Manage your friends list";
}

std::string FriendCommand::getUsage() const
{
    return "Usage: .friend <add/remove/list/clear> <name>";
}
