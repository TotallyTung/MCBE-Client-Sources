//
// Created by vastrakai on 10/31/2023.
//

#include "Hivestats.h"

void Hivestats::OnRun(std::vector<std::string> args) {
    auto player = Minecraft::ClientInstance->ThePlayer();

    // args check
    if (args.size() < 2) {
        AddCommandMessage("Usage: .hivestats <game> <player>");
        return;
    }

    std::string game = args[0];
    std::string name = args[1];

    std::string stats = Internet::GetHiveStats(game, name);

    Logger::Write("Hivestats", stats, Logger::LogType::Info);

    if (stats.empty()) {
        AddCommandMessage("Error: Could not find stats for " + name + " in " + game);
        return;
    }

    AddCommandMessage("&a" + name + "'s stats in " + game + ":&r\n" + stats);



}
