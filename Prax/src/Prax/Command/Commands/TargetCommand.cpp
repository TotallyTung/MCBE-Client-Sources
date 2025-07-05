//
// Created by vastrakai on 4/13/2024.
//

#include "TargetCommand.h"

#include <Prax/Module/Modules/Misc/AutoSnipe.h>
#include <Prax/Module/ModuleManager.h>

void TargetCommand::OnRun(std::vector<std::string> args) {
    if (args.size() < 1) {
        AddCommandMessage("&cUsage: .target <add/remove/list/clear> [player] &7[not enough arguments]");
        return;
    }

    auto autoSnipe = ModuleManager::GetModule<AutoSnipe>();
    if (autoSnipe == nullptr) {
        AddCommandMessage("&cAutoSnipe module not found!!");
        return;
    }

    std::string action = String::ToLower(args[0]);

    // if the action is not list or clear then validate the player
    if (action != "list" && action != "clear") {
        if (args.size() < 2) {
            AddCommandMessage("&cUsage: &6.target &c<add/remove> &c<player>&7 [not enough arguments, expected 2, got " + std::to_string(args.size()) + "]");
            return;
        }
    }

    if (action == "add") {
        // Add everything after the first two arguments
        std::string target = args[1];
        for (int i = 2; i < args.size(); i++) {
            target += " " + args[i];
        }

        // Check if this is a list of players seperated by commas
        if (target.find(",") != std::string::npos) {
            std::vector<std::string> targets = String::Split(target, ",");
            for (auto const& target : targets) {
                // remove any symbols from da player name
                std::string targetStr = target;
                targetStr.erase(std::remove_if(targetStr.begin(), targetStr.end(), ::ispunct), targetStr.end());

                // Remove any leading/trailing spaces
                targetStr = String::Trim(targetStr);

                AutoSnipe::Targets.emplace_back(targetStr);

                AddCommandMessage("&aAdded target: &6" + targetStr);
            }

            AddCommandMessage("&aAdded targets: &6" + target);
            Prax::Preferences->AutoSnipeTargets = AutoSnipe::Targets;
            PrefManager::Save(Prax::Preferences);
            return;
        }


        // remove any symbols from da player name
        target.erase(std::remove_if(target.begin(), target.end(), ::ispunct), target.end());

        AutoSnipe::Targets.emplace_back(target);
        AddCommandMessage("&aAdded target: &6" + target);
    } else if (action == "remove") {
        std::string target = args[1];
        for (int i = 2; i < args.size(); i++) {
            target += " " + args[i];
        }

        // remove any symbols from da player name
        target.erase(std::remove_if(target.begin(), target.end(), ::ispunct), target.end());

        auto it = std::find(AutoSnipe::Targets.begin(), AutoSnipe::Targets.end(), target);
        if (it != AutoSnipe::Targets.end()) {
            AutoSnipe::Targets.erase(it);
            AddCommandMessage("&aRemoved target: &6" + target);
        } else {
            AddCommandMessage("&cTarget not found: &6" + target);
        }
    } else if (action == "list") {
        std::string targets = "";

        for (auto const& target : AutoSnipe::Targets) {
            targets += target + ", ";
        }

        // remove the last comma
        targets = targets.substr(0, targets.size() - 2);

        // copy targets to clipboard
        Misc::SetClipboard(targets);

        AddCommandMessage("Targets: " + targets);
    } else if (action == "clear") {
        AutoSnipe::Targets.clear();
        AddCommandMessage("&eCleared targets successfully.");
    } else {
        AddCommandMessage("&cUsage: &6.target &c<add/remove/list/clear> [player] &7[invalid action: " + action + "]");
        return;
    }


    Prax::Preferences->AutoSnipeTargets = AutoSnipe::Targets;
    PrefManager::Save(Prax::Preferences);
}