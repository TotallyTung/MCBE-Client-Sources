//
// Created by vastrakai on 8/16/2024.
//

#include "StaffAlert.hpp"

#include <Features/Events/BaseTickEvent.hpp>
#include <SDK/Minecraft/World/Level.hpp>
#include <Utils/MiscUtils/DataStore.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>

// example: (URL: https://api.playhive.com/v0/game/all/main/FlareonRapier, Method: GET)
/*
{
  "main": {
    "UUID": "2abd2e6a-8127-36da-bbc4-516452f7e65f",
    "xuid": 2535418481650255,
    "username": "flareonrapier",
    "username_cc": "FlareonRapier",
    "rank": "REGULAR",
    "first_played": 1659627061,
    "hub_title_count": 1,
    "hub_title_unlocked": [
      "&eThe &bSky Warrior"
    ],
    "avatar_count": 0,
    "costume_count": 0,
    "hat_count": 0,
    "backbling_count": 0,
    "pets": [],
    "mounts": []
  }
}*/

static constexpr uint64_t rateLimitMs = 15 * 1000; // 15 seconds

void StaffAlert::onHttpResponse(HttpResponseEvent event)
{
    auto sender = static_cast<StaffAlert*>(event.mSender);
    auto originalRequest = static_cast<HttpRequest*>(event.mOriginalRequest);

    if (event.mStatusCode == 200)
    {
        nlohmann::json json = nlohmann::json::parse(event.mResponse);
        if (json.contains("main"))
        {
            auto main = json["main"];
            int64_t firstPlayed = main["first_played"].get<int64_t>();
            PlayerInfo player = PlayerInfo(main["username_cc"], main["rank"], firstPlayed);
            sender->mPlayerStore.mObjects.push_back(player);
        }
    } else if (event.mStatusCode == 404) {
        // Extract the name from the URL
        std::string name = originalRequest->mUrl.substr(originalRequest->mUrl.find_last_of('/') + 1);
        // Name is not found, so we add it to the cache with the rank "NICKED"
        PlayerInfo player = PlayerInfo(name, "NICKED", 0);
        sender->mPlayerStore.mObjects.push_back(player);
        spdlog::info("[StaffAlert] Player not found: {}, adding as rank 'NICKED'", name);
    } else if (event.mStatusCode == 429) {
        // Rate limited
        spdlog::warn("[StaffAlert] Rate limited, waiting {} seconds", rateLimitMs / 1000);
        ChatUtils::displayClientMessageRaw("§c§l» §r§cRate limited, waiting {} seconds", rateLimitMs / 1000);
        sender->mLastRateLimit = NOW;
    } else {
        spdlog::error("[StaffAlert] Request failed with status code: {}", event.mStatusCode);
    }

    if (sender->mSaveToDatabase.mValue) sender->mPlayerStore.save();

    // Don't delete the request here, it will be deleted by BaseTickEvent when the future is done
}


bool StaffAlert::isPlayerCached(const std::string& name) const
{
    return std::ranges::find_if(mPlayerStore.mObjects, [&name](const PlayerInfo& player) { return player.name == name; }) != mPlayerStore.mObjects.end();
}


const std::string& StaffAlert::getRank(const std::string& name)
{
    static const std::string unknown = "Unknown";

    for (const auto& player : mPlayerStore.mObjects)
    {
        if (player.name == name)
        {
            return player.rank;
        }
    }
    return unknown;
}

int64_t StaffAlert::getFirstJoined(const std::string& name) const
{
    for (const auto& player : mPlayerStore.mObjects)
    {
        if (player.name == name)
        {
            return player.getFirstJoined();
        }
    }

    return -1;
}

// Default ranks: REGULAR, PLUS
// This is not inclusive of YOUTUBE, TWITCH, HIVE_TEAM, etc.
static constexpr std::array ranks = { "REGULAR", "PLUS" };

void StaffAlert::makeRequest(const std::string& name)
{
    if (std::ranges::find(mRequestedPlayers, name) != mRequestedPlayers.end() || isPlayerCached(name))
    {
        return;
    }
    mRequestedPlayers.push_back(name);

    auto request = std::make_unique<HttpRequest>(HttpMethod::GET, "https://api.playhive.com/v0/game/all/main/" + name, "", "", &StaffAlert::onHttpResponse, this);
    int64_t now = NOW;
    mRequests.emplace_back( now, std::move(request) );
}

void StaffAlert::onEnable()
{
    if (mSaveToDatabase.mValue) mPlayerStore.load();
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &StaffAlert::onBaseTickEvent>(this);
}

void StaffAlert::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &StaffAlert::onBaseTickEvent>(this);
}

void StaffAlert::onBaseTickEvent(BaseTickEvent& event)
{
    static bool lastSaveState = mSaveToDatabase.mValue;
    if (lastSaveState != mSaveToDatabase.mValue)
    {
        lastSaveState = mSaveToDatabase.mValue;
        if (mSaveToDatabase.mValue) mPlayerStore.load();
    }

    //spdlog::info("[StaffAlert] Players: {}, Requests: {}, Events: {}", mPlayers.size(), mRequests.size(), mPlayerEvents.size());

    for (auto it = mRequests.begin(); it != mRequests.end();)
    {
        if (it->second->isDone())
        {
            it = mRequests.erase(it);
        } else {
            ++it;
        }
    }

    if (mLastRateLimit != 0 && NOW - mLastRateLimit < rateLimitMs)
    {
        return;
    }

    auto player = event.mActor;
    if (!player) return;

    // ---- Request handling ----
    static uint64_t lastSend = 0;
    static uint64_t timeBetweenRequests = 100;

    uint64_t now = NOW;

    if (now - lastSend > timeBetweenRequests)
    {
        lastSend = now;
        if (!mRequests.empty())
        {
            auto& request = mRequests.front();
            if (!request.second->mRequestSent)
            {
                request.second->sendAsync();
            }
        }
    }

    // ---- Event handling ----
    std::unordered_map<mce::UUID, PlayerListEntry>* playerList = player->getLevel()->getPlayerList();
    std::vector<std::string> playerNames;

    for (auto& entry : *playerList | std::views::values)
    {
        if (entry.mName.length() <= 17)
        {
            playerNames.emplace_back(entry.mName);
        }
    }

    static std::vector<std::string> lastPlayerNames = playerNames;

    for (auto& playerName : playerNames)
    {
        if (std::ranges::find(lastPlayerNames, playerName) == lastPlayerNames.end())
        {
            auto je = PlayerEvent{ PlayerEvent::Type::JOIN, playerName };
            mPlayerEvents.push_back(je);
        }
    }

    for (auto& playerName : lastPlayerNames)
    {
        if (std::ranges::find(playerNames, playerName) == playerNames.end())
        {
            auto le = PlayerEvent{ PlayerEvent::Type::LEAVE, playerName };
            mPlayerEvents.push_back(le);
        }
    }

    // Store the last player list
    lastPlayerNames = playerNames;

    // Process the events of cached players
    for (auto it = mPlayerEvents.begin(); it != mPlayerEvents.end();)
    {
        auto& daEvent = *it;
        if (isPlayerCached(daEvent.name))
        {
            std::string rank = getRank(daEvent.name);
            int64_t firstJoined = getFirstJoined(daEvent.name);

            bool isDefaultRank = std::ranges::find(ranks, rank) != ranks.end();

            if (firstJoined < 0)
            {
                it = mPlayerEvents.erase(it);
                continue;
            }

            std::string nameColor = isDefaultRank ? "§7" : "§c"; // Gray color for default ranks, Yellow color for custom ranks
            std::string spz = rank;
            if (spz == "NICKED") spz = "§cNICKED"; // Red color for nicked players

            // Process the JOIN event
            if (daEvent.type == PlayerEvent::Type::JOIN)
            {
                if (mShowRecentJoins.mValue) {
                    auto currentTime = std::chrono::system_clock::now();
                    auto playerJoinTime = std::chrono::system_clock::from_time_t(firstJoined);
                    auto duration = std::chrono::duration_cast<std::chrono::minutes>(currentTime - playerJoinTime);

                    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
                    auto minutes = duration - hours;

                    if (duration.count() <= 24 * 60) {  // Within 1 day
                        std::string timeStr;
                        if (hours.count() > 0) {
                            timeStr += std::to_string(hours.count()) + "h ";
                        }

                        timeStr += std::to_string(minutes.count()) + "m";

                        std::string newPlayerStr = "§aRecent joined player detected: §f" + daEvent.name + " (" + timeStr + ")";
                        ChatUtils::displayClientMessage(newPlayerStr);
                    }
                }

                if ((!isDefaultRank && mStaffOnly.mValue) || !mStaffOnly.mValue) {
                    ChatUtils::displayClientMessageRaw("§a§l» §r§6[{}§6]{} {} §ajoined.", spz, nameColor, daEvent.name);
                    if (mShowNotifications.mValue) NotifyUtils::notify("[" + rank + "] " + daEvent.name + " joined", 5.0f, Notification::Type::Info);
                    if (mPlaySound.mValue) ClientInstance::get()->playUi("random.orb", 1.0f, 1.0f);
                }
            }
            else if (daEvent.type == PlayerEvent::Type::LEAVE)
            {
                if ((!isDefaultRank && mStaffOnly.mValue) || !mStaffOnly.mValue) {
                    ChatUtils::displayClientMessageRaw("§c§l» §r§6[{}§6]{} {} §cleft.", spz, nameColor, daEvent.name);
                    if (mShowNotifications.mValue) NotifyUtils::notify("[" + rank + "] " + daEvent.name + " left", 5.0f, Notification::Type::Info);
                }
            }

            it = mPlayerEvents.erase(it);
        }
        else
        {
            makeRequest(daEvent.name);
            ++it;
        }
    }
}