//
// Created by Flash on 8/23/2023.
//

#include "Config.h"
#include <Prax/Config/ConfigManager.h>
#include <Prax/Utils/String.h>

void Config::ShowUsage() {
    AddCommandMessage("&eUsage: &b.config <load/save/default/delete/list> <name>");
    AddCommandMessage("&bload <name> &7- &aLoads a config");
    AddCommandMessage("&bsave <name> &7- &aSaves a config");
    AddCommandMessage("&bdefault <name> &7- &aSets a config as the default");
    AddCommandMessage("&bdelete <name> &7- &aDeletes a config");
    AddCommandMessage("&blist &7- &aLists all configs");

}

void Config::OnRun(std::vector<std::string> args) {
    // Check if the user has provided enough arguments
    if (args.empty()) {
        ShowUsage();
        return;
    }



    if (String::StartsWith(args[0], "list"))
    {
        AddCommandMessage("Configs:");
        for (const auto &entry : std::filesystem::directory_iterator(FileSystem::ConfigDirectory)) {
            if (entry.path().extension() == ".json") {
                AddCommandMessage(entry.path().filename().string());
            }
        }

        return;
    }

    if (args.size() < 2) {
        ShowUsage();
        return;
    }

    if (String::ContainsIgnoreCase(args[1], ".json"))
    {
        // Remove the .json from the end of the file
        args[1] = args[1].substr(0, args[1].size() - 5);
    }

    if (String::StartsWith(args[0], "l")) {
        bool result = ConfigManager::Load(args[1]);
        if (result) {
            AddCommandMessage("&aLoaded config &b" + args[1] + "&a!");
        } else {
            // Check if the config exists. If it doesn't, tell the user.
            std::string path = FileSystem::ConfigDirectory + "\\" + args[1] + ".json";
            if (!std::filesystem::exists(path)) AddCommandMessage("&7[&cError&7] &cConfig &b" + args[1] + "&c does not exist!");
            else AddCommandMessage("&7[&cError&7] &cFailed to load config " + args[1] + "!");
        }
    } else if (String::StartsWith(args[0], "s"))
    {
        // Check if arg[1] is overwrite
        if (args.size() == 3 && String::StartsWith(args[2], "over")) {
            ConfigManager::Save(args[1]);
            AddCommandMessage("&aSaved config &e" + args[1] + "&a!");
            return;
        }

        std::string path = FileSystem::ConfigDirectory + "\\" + args[1] + ".json";
        // If the config exists and the config we're saving over isn't the current config, don't save
        if (std::filesystem::exists(path) && ConfigManager::CurrentConfig != args[1]) {
            AddCommandMessage("&7[&cError&7] &cConfig &b" + args[1] + "&c already exists! Use &b.config save " + args[1] + " overwrite &cto overwrite it!");
            return;
        }
        ConfigManager::Save(args[1]);
        AddCommandMessage("&aSaved config &b" + args[1] + "&a!");
    } else if (String::StartsWith(args[0], "def"))
    {
        if (args[1] == "reset") {
            ConfigManager::SetDefault("");
            Logger::Write("Config", "Setting default config to default");
            PrefManager::Save(Prax::Preferences);
            AddCommandMessage("&aSuccessfully reset the default config!");
            return;
        }
        Logger::Write("Config", "Setting default config to " + args[1]);
        ConfigManager::SetDefault(args[1]);
        Logger::Write("Config", "Saving preferences");
        PrefManager::Save(Prax::Preferences);
        AddCommandMessage("&aSuccessfully set &b" + args[1] + "&a as the default config!");
    } else if (String::StartsWith(args[0], "del"))
    {
        std::string path = FileSystem::ConfigDirectory + "\\" + args[1] + ".json";
        if (std::filesystem::exists(path)) {
            std::filesystem::remove(path);
            AddCommandMessage("&aDeleted config &b" + args[1] + "&a!)");
        } else {
            AddCommandMessage("&7[&cError&7] &cConfig " + args[1] + " does not exist!");
        }
    }
    else {
        ShowUsage();
    }
}
