//
// Created by Flash on 8/23/2023.
//

#include "ConfigManager.h"

#include <Prax/Utils/System/FileSystem.h>
#include <Prax/Module/ModuleManager.h>

#include <fstream>
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <nlohmann/json.hpp>
#include <Prax/Module/Modules/Visual/HUD.h>

using json = nlohmann::json;

void ConfigManager::Init() {
}

void ConfigManager::Shutdown() {
}

bool ConfigManager::Load(const std::string &name) {
    Loading = true;

    try {
        Logger::Write("Configs", "Loading config " + name + "...");


        std::ifstream i(FileSystem::ConfigDirectory + "\\" + name + ".json");
        // Check if the config exists
        if (!i.good()) {
            Logger::Write("Configs", "Config " + name + " does not exist", Logger::LogType::Warning);
            Loading = false;
            return false;
        }

        json j;
        i >> j;

        for (auto mod: j["modules"]) {
            for (const auto mod2: ModuleManager::Modules) {
                try
                {
                    if (mod["name"] == mod2->Name) {
                        mod2->SetToggleState(mod["enabled"]);
                        mod2->KeyId = mod["keybind"];
                        mod2->HideInArraylist = mod["hideInArraylist"];

                        for (const auto set: mod2->Settings) {

                            if (mod["settings"].find(set->Name) == mod["settings"].end()) continue;

                            switch (set->Type) {
                                case Bool: {
                                    auto *boolSetting = static_cast<BoolSetting*>(set);
                                    boolSetting->Enabled = mod["settings"][set->Name]["boolValue"];
                                    break;
                                }
                                case Number: {
                                    auto *numberSetting = static_cast<NumberSetting*>(set);
                                    numberSetting->Value = mod["settings"][set->Name]["numberValue"];
                                    break;
                                }
                                case Mode: {
                                    auto *modeSetting = static_cast<ModeSetting*>(set);
                                    modeSetting->SelectedIndex = mod["settings"][set->Name]["modeValue"];
                                        // make sure the selected index is valid
                                    if (modeSetting->SelectedIndex >= modeSetting->Modes.size()) {
                                        Logger::Write("ConfigManager", "Invalid mode index for " + mod2->Name + " " + set->Name + ", setting to 0", Logger::LogType::Warning);
                                        modeSetting->SelectedIndex = 0;
                                    }
                                    break;
                                }
                            }

                        }
                    }
                } catch (std::exception &e) {
                    Logger::Write("Configs",
                                  "Failed to load module " + mod["name"].get<std::string>() + ": " + e.what());
                }
            }
        }
        Loading = false;
        CurrentConfig = name;
        return true;
    }
    catch (std::exception& e) {
        Logger::Write("Configs", "Failed to load config " + name + ": " + e.what());
        Loading = false;
        CurrentConfig = name;
        return false;
    }
}

void ConfigManager::Save(const std::string &name) {
    Logger::Write("Configs", "Saving config " + name + "...");
    try {

        FileSystem::CreateDirectoryP(FileSystem::ConfigDirectory);
        FileSystem::DeleteFileF(FileSystem::ConfigDirectory + "\\" + name + ".json");

        json j;
        j["modules"] = json::array();

        for (const auto mod : ModuleManager::Modules) {
            json modJson;
            modJson["name"] = mod->Name;
            modJson["enabled"] = mod->Enabled;
            modJson["keybind"] = mod->KeyId;
            modJson["hideInArraylist"] = mod->HideInArraylist;

            for (const auto set : mod->Settings)
            {
                switch (set->Type)
                {
                    case Bool: {
                        auto* boolSetting = static_cast<BoolSetting*>(set);
                        modJson["settings"][set->Name]["boolValue"] = boolSetting->Enabled;
                        break;
                    }
                    case Number: {
                        auto* numberSetting = static_cast<NumberSetting*>(set);
                        modJson["settings"][set->Name]["numberValue"] = numberSetting->Value;
                        break;
                    }
                    case Mode: {
                        auto* modeSetting = static_cast<ModeSetting*>(set);
                        modJson["settings"][set->Name]["modeValue"] = modeSetting->SelectedIndex;
                        break;
                    }
                }

            }
            j["modules"].push_back(modJson);
        }

        std::ofstream o(FileSystem::ConfigDirectory + "\\" + name + ".json");
        o << std::setw(4) << j.dump() << std::endl;
        o.close();
        CurrentConfig = name;
    }
    catch (std::exception& e) {
        Logger::Write("Configs", "Failed to save config " + name + ": " + e.what());
    }
}

void ConfigManager::SetDefault(const std::string& name)
{
    if (name == "") {
        Logger::Write("Configs", "Resetting default config...");
        Prax::Preferences->DefaultConfig = "";
        return;
    }
    // Make sure the config exists
    if (!FileSystem::FileExists(FileSystem::ConfigDirectory + "\\" + name + ".json")) {
        Logger::Write("Configs", "Config " + name + " does not exist", Logger::LogType::Warning);
        return;
    }

    Logger::Write("Configs", "Setting default config " + name + "...");

    Prax::Preferences->DefaultConfig = name;
}
