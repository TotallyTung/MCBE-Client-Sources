//
// Created by alteik on 01/09/2024.
//

#include "Teams.hpp"
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Options.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Actor/Components/FlagComponent.hpp>
#include <SDK/Minecraft/Rendering/GuiData.hpp>


void Teams::onEnable()
{
}

void Teams::onDisable()
{
}

bool Teams::isOnTeam(class Actor* actor)
{
    if (!this)
    {
        spdlog::error("Teams module is null");
        return false;
    }
    if (!mEnabled) return false;

    auto localPlayer = ClientInstance::get()->getLocalPlayer();
    if (!localPlayer) return false;

    std::string playerName = localPlayer->getNameTag();
    std::string actorName = actor->getNameTag();

    if (playerName.empty() || actorName.empty()) return false;

    auto cleanName = [](std::string &name) {
        constexpr std::string tags[] = {"§r", "§l"};
        for (const auto &tag : tags) {
            size_t pos;
            while ((pos = name.find(tag)) != std::string::npos) {
                name.erase(pos, tag.length());
            }
        }
    };

    cleanName(playerName);
    cleanName(actorName);

    size_t playerTeamPos = playerName.find("§");
    if (playerTeamPos == std::string::npos) return false;

    std::string playerTeam = playerName.substr(playerTeamPos + 2, 1);

    size_t actorTeamPos = actorName.find("§");
    if (actorTeamPos != std::string::npos) {
        std::string actorTeam = actorName.substr(actorTeamPos + 2, 1);
        return actorTeam == playerTeam;
    }

    return false;
}
