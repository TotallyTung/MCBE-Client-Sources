#include <algorithm>
#include <string>
#include <filesystem>
#include "../Command.h"
#include "Friend.h"
#include "../../CommandManager.h"
#include "../../../Client.h"
#include "../../../../Utils/TargetUtils.h"

Friend::Friend() : Command("Friend", "Friends add or delete", { "f", "fri", "friends" }, "<u got some stupititys>") {}

void Friend::execute(const std::vector<std::string>& args) {
    static Notifications* NotificationsMod = (Notifications*)client->moduleMgr->getModule("Notifications");
    if (args.size() < 2) {
        mc.DisplayClientMessage("[%sMelody%s] %sInvalid command! Usage: .(f,fri,friends) <add/a> <playername> | <delete/d> <playername> | show", DARK_PURPLE, WHITE, RED);
        return;
    }
    std::string subCommand = args[1];
    std::string playerName;
    if (args.size() > 2) {
        playerName = std::accumulate(args.begin() + 2, args.end(), std::string(), [](const std::string& a, const std::string& b) {
            return a + (a.length() > 0 ? " " : "") + b;
            });
    }

    if (subCommand == "add" || subCommand == "a") {
        std::string sanitizedPlayerName = Utils::sanitize(playerName);
        if (std::find(TargetUtils::Friend.begin(), TargetUtils::Friend.end(), sanitizedPlayerName) == TargetUtils::Friend.end()) {
            TargetUtils::Friend.push_back(sanitizedPlayerName);
            mc.DisplayClientMessage("[%sMelody%s] %s%s added to friends list!", DARK_PURPLE, WHITE, GREEN, sanitizedPlayerName.c_str());
        }
        else {
            mc.DisplayClientMessage("[%sMelody%s] %s%s is already in friends list!", DARK_PURPLE, WHITE, YELLOW, sanitizedPlayerName.c_str());
        }
    }
    else if (subCommand == "delete" || subCommand == "d") {
        std::string sanitizedPlayerName = Utils::sanitize(playerName);
        auto it = std::find(TargetUtils::Friend.begin(), TargetUtils::Friend.end(), sanitizedPlayerName);
        if (it != TargetUtils::Friend.end()) {
            TargetUtils::Friend.erase(it);
            mc.DisplayClientMessage("[%sMelody%s] %s%s removed from friends list!", DARK_PURPLE, WHITE, GREEN, sanitizedPlayerName.c_str());
        }
        else {
            mc.DisplayClientMessage("[%sMelody%s] %s%s is not in friends list!", DARK_PURPLE, WHITE, YELLOW, sanitizedPlayerName.c_str());
        }
    }
    else if (subCommand == "show" || subCommand == "s") {
        mc.DisplayClientMessage("[%sMelody%s] %sFriends List:", DARK_PURPLE, WHITE, GREEN);
        for (const auto& friendName : TargetUtils::Friend) {
            mc.DisplayClientMessage(friendName.c_str());
        }
    }
    else if (subCommand == "clear" || subCommand == "c") {
        TargetUtils::Friend.clear();
        mc.DisplayClientMessage("[%sMelody%s] %sFriends list cleared!", DARK_PURPLE, WHITE, GREEN);
    }
    else {
        mc.DisplayClientMessage("[%sMelody%s] %sInvalid command!", DARK_PURPLE, WHITE, RED);
    }
}
