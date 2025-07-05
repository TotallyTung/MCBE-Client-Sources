//
// Created by vastrakai on 9/24/2023.
//

#include "CheckAllUsers.h"

void CheckAllUsers::OnRun(std::vector<std::string> args) {
    auto player = Minecraft::ClientInstance->ThePlayer();

    std::vector<std::string> invalidUsers;

    for (const auto& pair : player->level->playerList) {
        auto first = pair.first;
        auto second = pair.second;

        std::string name = second.name;

        int statusCode = Internet::CheckUser(name);
        if (statusCode != 200) {
            invalidUsers.push_back(name);
        }
    }

    if (invalidUsers.size() == 0) {
        AddCommandMessage("All users are valid");
    } else {
        std::string message = "Invalid users: ";
        for (const auto& user : invalidUsers) {
            message += user + ", ";
        }

        // Remove the last comma
        message = message.substr(0, message.size() - 2);

        AddCommandMessage(message);
    }



}
