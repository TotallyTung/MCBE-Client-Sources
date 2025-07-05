//
// Created by vastrakai on 1/1/2024.
//

#include "CheckAllStats.h"

void CheckAllStats::OnRun(std::vector<std::string> args)
{
    std::thread t(Exec);
    t.detach();
}

void CheckAllStats::Exec()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    // Get the hive stats of every player in the
    std::vector<std::string> hiveStats;

    for (auto& [_, player] : player->level->playerList)
    {
        int64_t firstPlayed = Internet::GetFirstPlayed("sky", player.name);

        if (firstPlayed == 0) continue;

        // Continue if the played time is more than 5 days ago
        if (firstPlayed < (time(0) - (60 * 60 * 24 * 5)))
        {
            Logger::Write("CheckAllStats", "Skipping " + player.name + " because they joined more than 5 days ago", Logger::LogType::Info);
            continue;
        }

        std::string firstPlayedTime = String::FormatTime(firstPlayed, "%m/%d/%Y %H:%M:%S");
        int64_t msSinceFirstPlayed = time(0) - firstPlayed;
        Logger::Write("CheckAllStats", "msSinceFirstPlayed: " + std::to_string(msSinceFirstPlayed), Logger::LogType::Info);
        std::string relativeFirstPlayedTime = String::RelativeTime(msSinceFirstPlayed);

        Logger::Write("CheckAllStats", "Adding " + player.name + ", first played: " + firstPlayedTime, Logger::LogType::Info);

        std::string playerName = player.name;
        std::string xuid = player.XUID;

        hiveStats.push_back("§cPlayer: §b" + playerName + "§c. §eFirst played: §b" + firstPlayedTime + " &7(" + relativeFirstPlayedTime + " ago)&r");
    }
    if (hiveStats.empty()) {
        //AddCommandMessage("No players have joined in the past 5 days in this lobby");
        player->addChatMessage("No players have joined in the past 5 days in this lobby");
    } else {
        std::string message = "";
        for (const auto& stat : hiveStats) {
            message += stat + "\n";
        }

        //AddCommandMessage(message);
        player->addChatMessage(message);
    }
}
