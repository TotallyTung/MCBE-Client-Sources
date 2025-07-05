//
// Created by Flash on 9/26/2023.
//

#include "PrefManager.h"

Preferences* PrefManager::Load() {

    // Load prefs from file
    const std::string path = FileSystem::Directory + "\\prefs.json";


    auto* prefs = new Preferences();

    if (!FileSystem::FileExists(path)) {
        Save(prefs);
        return prefs;
    }

    // Read json from file
    std::ifstream i(path);
    // Check if the config exists
    if (!i.good()) {
        Logger::Write("PrefManager", "File " + path + " does not exist");
        return prefs;
    }

    // Check if the file is empty
    if (i.peek() == std::ifstream::traits_type::eof())
    {
        Logger::Write("PrefManager", "Prefs file is empty");
        return prefs;
    }

    nlohmann::json j;
    i >> j;

    // if you add settings to this please add them to the end so other settings are loaded!!!!!!!!
    try {
        if (j.contains("d3d11Fallback")) {
            prefs->d3d11Fallback = j["d3d11Fallback"];
        }

        prefs->IRC.Name = j["IRC"]["Name"];
        prefs->IRC.Channel = j["IRC"]["Channel"];
        prefs->IRC.ShowJoins = j["IRC"]["ShowJoins"];
        prefs->Module.StreamerName = j["Module"]["StreamerName"];
        prefs->Module.Friends = j["Module"]["Friends"];
        if (j.contains("DefaultConfig")) {
            prefs->DefaultConfig = j["DefaultConfig"];
        }
        if (j.contains("ClientName")) {
            prefs->ClientName = j["ClientName"];
        }
        if (j.contains("AutoSnipeTargets")) {
            prefs->AutoSnipeTargets = j["AutoSnipeTargets"];
        }
    } catch (std::exception e) {
        Logger::Write("PrefManager", "Error parsing prefs file: " + std::string(e.what()));

    }


    return prefs;
}

void PrefManager::Save(Preferences *prefs) {

    if (!prefs) {
        Logger::Write("PrefManager", "Error saving prefs: prefs is null");
        return;
    }

    // Save prefs to file
    const std::string path = FileSystem::Directory + "\\prefs.json";

    FileSystem::DeleteFileF(path);
    FileSystem::CreateFileF(path);

    nlohmann::json j;

    try {
        j["d3d11Fallback"] = prefs->d3d11Fallback;
        j["IRC"] = nlohmann::json::object();
        j["IRC"]["Name"] = prefs->IRC.Name;
        j["IRC"]["Channel"] = prefs->IRC.Channel;
        j["IRC"]["ShowJoins"] = prefs->IRC.ShowJoins;
        j["Module"]["StreamerName"] = prefs->Module.StreamerName;
        j["Module"]["Friends"] = prefs->Module.Friends;
        j["DefaultConfig"] = prefs->DefaultConfig;
        j["ClientName"] = prefs->ClientName;
        j["AutoSnipeTargets"] = prefs->AutoSnipeTargets;
    } catch (std::exception e) {
        Logger::Write("PrefManager", "Error serializing prefs: " + std::string(e.what()));
    }

    // Serialize prefs to json
    // Write json to file
    std::ofstream o(path);
    o << std::setw(4) << j.dump() << std::endl;
    o.close();
}