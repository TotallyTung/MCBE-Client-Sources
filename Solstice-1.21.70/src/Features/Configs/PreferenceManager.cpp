//
// Created by vastrakai on 7/12/2024.
//

#include "PreferenceManager.hpp"

#include <fstream>
#include <Utils/FileUtils.hpp>

#include "spdlog/spdlog.h"

std::shared_ptr<Preferences> PreferenceManager::load()
{
    std::string path = FileUtils::getSolsticeDir() + "preferences.json";

    auto prefs = std::make_shared<Preferences>();

    if (!FileUtils::fileExists(path))
    {
        spdlog::warn("Preferences file not found, creating new one.");
        save(prefs);
        return prefs;
    }

    std::ifstream file(path);
    if (!file.good()) {
        spdlog::error("Failed to open preferences file!");
        return prefs;
    }

    if (file.peek() == std::ifstream::traits_type::eof())
    {
        spdlog::warn("Preferences file is empty, creating new one.");
        save(prefs);
        return prefs;
    }

    nlohmann::json j;
    file >> j;
    file.close();

    try
    {
        prefs->mDefaultConfigName = j["DefaultConfigName"];
        prefs->mFriends = j["Friends"].get<std::vector<std::string>>();
        prefs->mFallbackToD3D11 = j["FallbackToD3D11"];
        prefs->mEnforceDebugging = j["EnforceDebugging"];
        prefs->mIrcName = j["IrcName"];
        prefs->mStreamerName = j["StreamerName"];
    }
    catch (std::exception e)
    {
        spdlog::error("Error parsing preferences file: {}", e.what());
        save(prefs);
        return prefs;
    }

    spdlog::info("Successfully loaded preferences!");
    return prefs;
}

void PreferenceManager::save(const std::shared_ptr<Preferences>& prefs)
{
    if (!prefs)
    {
        spdlog::error("Failed to save preferences, preferences object is null.");
        return;
    }

    std::string path = FileUtils::getSolsticeDir() + "preferences.json";

    FileUtils::deleteFile(path);
    FileUtils::createFile(path);

    nlohmann::json j;
    try
    {
        j["DefaultConfigName"] = prefs->mDefaultConfigName;
        j["Friends"] = prefs->mFriends;
        j["FallbackToD3D11"] = prefs->mFallbackToD3D11;
        j["EnforceDebugging"] = prefs->mEnforceDebugging;
        j["IrcName"] = prefs->mIrcName;
        j["StreamerName"] = prefs->mStreamerName;
    } catch (std::exception& e)
    {
        spdlog::error("Error saving preferences: {}", e.what());
        return;
    }

    std::ofstream file(path);
    file << j.dump(4);
    file.close();
    spdlog::info("Successfully saved preferences!");
}
