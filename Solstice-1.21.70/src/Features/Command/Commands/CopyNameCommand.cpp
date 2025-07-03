//
// Created by ssi on 10/20/2024.
//

#include "CopyNameCommand.hpp"

#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <Utils/GameUtils/ChatUtils.hpp>

void CopyNameCommand::execute(const std::vector<std::string>& args)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    std::string playerName = player->getLocalName();

    if (playerName.empty()) {
        ChatUtils::displayClientMessage("§cUnable to copy gamertag: player name is unknown.");
        return;
    }

    std::string toCopy;
    if (playerName.find(' ') != std::string::npos) {
        toCopy = "/f add \"" + playerName + "\"";
    } else {
        toCopy = "/f add " + playerName;
    }

    ImGui::SetClipboardText(toCopy.c_str());
    ChatUtils::displayClientMessage("§aGamertag §b" + playerName + "§a has been copied to your clipboard!");
}

std::vector<std::string> CopyNameCommand::getAliases() const
{
    return { "copy" };
}

std::string CopyNameCommand::getDescription() const
{
    return "Copies your gamertag to your clipboard";
}

std::string CopyNameCommand::getUsage() const
{
    return "Usage: .copyname";
}