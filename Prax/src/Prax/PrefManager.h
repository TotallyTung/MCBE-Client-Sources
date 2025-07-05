//
// Created by Flash on 9/26/2023.
//

#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <Prax/Utils/System/FileSystem.h>
#include <Prax/Utils/System/Logger.h>
#include <fstream>

struct IRCPrefs {
    std::string Name = "Anonymous";
    std::string Channel = "global";
    bool ShowJoins = true;
};

struct ModulePrefs {
public:
    std::string StreamerName = "Catgirl";
    std::vector<std::string> Friends = std::vector<std::string>();
};

struct Preferences {
    bool d3d11Fallback = false;
    IRCPrefs IRC;
    ModulePrefs Module;
    std::string DefaultConfig = ""; // The default config to load on startup. (Blank for none)
    std::string ClientName = "catgirl";
    std::vector<std::string> AutoSnipeTargets = {};
};

class PrefManager {
public:
    static Preferences* Load();
    static void Save(Preferences* prefs);
};
