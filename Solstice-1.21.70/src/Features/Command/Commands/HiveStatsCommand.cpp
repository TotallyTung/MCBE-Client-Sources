//
// Created by vastrakai on 8/17/2024.
//

#include "HiveStatsCommand.hpp"

#include <spdlog/fmt/bundled/chrono.h>

void HiveStatsCommand::onStatsReceived(HttpResponseEvent event)
{
    try
    {
        auto instance = static_cast<HiveStatsCommand*>(event.mSender);
        auto request = instance->mRequest.get();
        const auto response = event.mResponse;

        if (event.mStatusCode != 200) {
            ChatUtils::displayClientMessage("§cFailed to fetch stats [{}]", event.mStatusCode);
            instance->mIsRequesting = false;
            return;
        }
        // Extract the username from the URL
        auto username = request->mUrl.substr(request->mUrl.find_last_of('/') + 1);

        // Remove non alphanumeric characters from the username
        std::erase_if(username, [](char c) { return !std::isalnum(c); });

        // Remove the username from the URL (erase everything after the last / including the /)
        request->mUrl.erase(request->mUrl.find_last_of('/'));
        // Extract the gamemode from the URL
        const auto gamemode = request->mUrl.substr(request->mUrl.find_last_of('/') + 1);

        spdlog::info("Stats received for {} in gamemode {}", username, gamemode);
        spdlog::info("Response: {}", response);

        // Process the json response
        nlohmann::json json = nlohmann::json::parse(response);

        // Remove the UUID if it exists
        if (json.contains("UUID")) json.erase("UUID");

        std::string output = "§6Stats for §e" + username + "§6 in gamemode §e" + gamemode + "§6:\n";
        // Go through each key in the json object
        for (auto& [k, val] : json.items())
        {
            if (k == "main")
            {
                // process each key in the main object
                for (auto& [k2, val2] : val.items())
                {
                    nlohmann::json value = val2;
                    std::string key = k2;
                    if (k2 == "first_played")
                    {
                        // The first_played is a string, but it contains a number
                        // Convert it to a date string
                        int64_t timestamp = std::stoll(value.dump());
                        auto time = std::chrono::system_clock::from_time_t(timestamp);
                        auto timeStr = fmt::format("{:%m/%d/%Y %H:%M:%S}", time);
                        // Get a relative time and append it to the string
                        timeStr += " (" + StringUtils::getRelativeTime(time) + ")";
                        value = timeStr;
                    }

                    // Process the key string to make it more readable
                    key = StringUtils::replaceAll(key, "_", " ");

                    std::string valstr = value.dump();
                    if (valstr.front() == '"' && valstr.back() == '"')
                    {
                        valstr = valstr.substr(1, valstr.size() - 2);
                    }

                    // Display the key and value
                    output += "§6- §e" + key + ": §7" + valstr + "\n";
                }

                continue;

            }
            nlohmann::json value = val;
            if (k == "first_played")
            {
                // The first_played is a string, but it contains a number
                // Convert it to a date string
                int64_t timestamp = std::stoll(value.dump());
                auto time = std::chrono::system_clock::from_time_t(timestamp);
                auto timeStr = fmt::format("{:%m/%d/%Y %H:%M:%S}", time);
                // Get a relative time and append it to the string
                timeStr += " (" + StringUtils::getRelativeTime(time) + ")";
                value = timeStr;
            }

            std::string key = k;
            // Process the key string to make it more readable
            key = StringUtils::replaceAll(key, "_", " ");

            std::string valstr = value.dump();
            if (valstr.front() == '"' && valstr.back() == '"')
            {
                valstr = valstr.substr(1, valstr.size() - 2);
            }

            // Display the key and value
            output += "§6- §e" + key + ": §7" + valstr + "\n";
        }

        // Remove the last newline character
        output.pop_back();
        ChatUtils::displayClientMessage(output);

        spdlog::info("Finished processing stats for {} in gamemode {}", username, gamemode);
        instance->mIsRequesting = false;
    } catch (const nlohmann::json::exception& e) {
        spdlog::error("Failed to parse json response: {}", e.what());
        ChatUtils::displayClientMessage("§cFailed to parse json response: {}", e.what());
    } catch (const std::exception& e) {
        spdlog::error("An error occurred: {}", e.what());
        ChatUtils::displayClientMessage("§cAn error occurred: {}", e.what());
    } catch (...) {
        spdlog::error("An unknown error occurred");
        ChatUtils::displayClientMessage("§cAn unknown error occurred");
    }
}

void HiveStatsCommand::execute(const std::vector<std::string>& args)
{
    if (args.size() < 3)
    {
        ChatUtils::displayClientMessage("§cUsage: {}", getUsage());
        return;
    }

    const auto gamemode = args[1];

    // for player names with spaces
    std::string player = "";
    for (size_t i = 2; i < args.size(); ++i)
    {
        player += args[i];
        if (i + 1 < args.size())
        {
            player += " ";
        }
    }

    if (mIsRequesting || mRequest && !mRequest->isDone())
    {
        ChatUtils::displayClientMessage("§cPlease wait for the previous request to finish!");
        return;
    }

    mIsRequesting = true;

    if (mRequest) mRequest.release(); // Release the request if it exists

    auto request = std::make_unique<HttpRequest>(HttpMethod::GET, "https://api.playhive.com/v0/game/all/" + gamemode + "/" + player, "", "", &HiveStatsCommand::onStatsReceived, this);
    mRequest = std::move(request);
    spdlog::info("Fetching stats for {} in gamemode {}", player, gamemode);
    mRequest->sendAsync();


}

std::vector<std::string> HiveStatsCommand::getAliases() const
{
    return { "hs", "stats" };
}

std::string HiveStatsCommand::getDescription() const
{
    return "Displays stats of a player on the Hive";
}

std::string HiveStatsCommand::getUsage() const
{
    return ".hivestats <gamemode> <player>";
}
