// Created by Tony on 2024-10-10 10:01:54

#include <vector>
#include <string>
#include "PlayerList.h"
#include "../../CommandManager.h"
#include "../../../Client.h"

PlayerList::PlayerList() : Command("PlayerList", "List all players on the server", { "playerlist" }, "<now>") {}

void PlayerList::execute(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        mc.DisplayClientMessage("[%sMelody%s] %sInvalid command! Usage: .playerlist <now>", DARK_PURPLE, WHITE, RED);
        return;
    }

    std::string subCommand = args[1];
    if (subCommand == "now") {
        Level* level = mc.getLocalPlayer()->getLevel();
        if (level) {
            const auto& playerMap = level->getPlayerMap();

            mc.DisplayClientMessage("[%sMelody%s] %sPlayers online:", DARK_PURPLE, WHITE, GREEN);

            std::vector<std::string> playerNames;
            for (const auto& pair : playerMap) {
                playerNames.push_back(pair.second.name);
            }

            // Sort the names alphabetically
            std::sort(playerNames.begin(), playerNames.end());

            for (const auto& playerName : playerNames) {
                mc.DisplayClientMessage("%s- %s", WHITE, playerName.c_str());
            }
        }
        else {
            mc.DisplayClientMessage("[%sMelody%s] %sUnable to access player list.", DARK_PURPLE, WHITE, RED);
        }
    }
    else {
        mc.DisplayClientMessage("[%sMelody%s] %sInvalid command! Usage: .playerlist <now>", DARK_PURPLE, WHITE, RED);
    }
}