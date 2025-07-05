//
// Created by vastrakai on 9/7/2023.
//

#include "Teams.h"

bool Teams::TeamsEnabled = false;

void Teams::OnEnable() {
    Teams::TeamsEnabled = true;
}

void Teams::OnDisable() {
    Teams::TeamsEnabled = false;
}

// This is a static func
bool Teams::IsOnTeam(Actor *actor) {
    if (!TeamsEnabled) return false;
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return false;
    std::string playerName = player->_getNameTag();
    std::string actorName = actor->_getNameTag();
    if (playerName.empty() || actorName.empty()) return false;
    if (playerName.find("§r") != std::string::npos) playerName.erase(playerName.find("§r"), 2);
    if (actorName.find("§r") != std::string::npos) actorName.erase(actorName.find("§r"), 2);
    if (playerName.find("§l") != std::string::npos) playerName.erase(playerName.find("§l"), 2);
    if (actorName.find("§l") != std::string::npos) actorName.erase(actorName.find("§l"), 2);

    std::string playerTeam = playerName.substr(playerName.find("§") + 2, 1);

    if (actorName.contains("§")) {
        std::string actorTeam = actorName.substr(actorName.find("§") + 2, 1);
        return actorTeam == playerTeam;
    }
    return false;
}
