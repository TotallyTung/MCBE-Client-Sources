//
// Created by vastrakai on 7/22/2024.
//

#include "GamemodeCommand.hpp"

#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>

void GamemodeCommand::execute(const std::vector<std::string>& args)
{
    if (args.size() != 2)
    {
        ChatUtils::displayClientMessage("§c" + getUsage());
        return;
    }

    // 0 = survival, 1 = creative, 2 = adventure, 6 = spectator (bedrock moment)

    const std::string& mode = args[1];

    auto player = ClientInstance::get()->getLocalPlayer();

    if (mode == "0" || mode == "survival")
    {
        player->setGameType(0);
        ChatUtils::displayClientMessage("§aSet gamemode to §6survival§a!");
    }
    else if (mode == "1" || mode == "creative")
    {
        player->setGameType(1);
        ChatUtils::displayClientMessage("§aSet gamemode to §6creative§a!");
    }
    else if (mode == "2" || mode == "adventure")
    {
        player->setGameType(2);
        ChatUtils::displayClientMessage("§aSet gamemode to §6adventure§a!");
    }
    else if (mode == "6" || mode == "3" || mode == "spectator")
    {
        player->setGameType(6);
        ChatUtils::displayClientMessage("§aSet gamemode to §6spectator§a!");
    }
    else
    {
        ChatUtils::displayClientMessage("§cInvalid gamemode!");
    }
}

std::vector<std::string> GamemodeCommand::getAliases() const
{
    return { "gm" };
}

std::string GamemodeCommand::getDescription() const
{
    return "Clientsidedly changes your gamemode";
}

std::string GamemodeCommand::getUsage() const
{
    return "Usage: .gamemode <mode>";
}
