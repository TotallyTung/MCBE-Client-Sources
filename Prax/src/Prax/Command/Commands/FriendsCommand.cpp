//
// Created by vastrakai on 9/22/2023.
//

#include <Prax/Utils/String.h>
#include "FriendsCommand.h"

void FriendsCommand::OnRun(std::vector<std::string> args) {

        // Check the arguments
        // usage is .friends <action> [name]
        if (args.empty()) {
            AddCommandMessage("&cUsage: .friend <add|remove|list|clear> [name]");
            return;
        }

        // Get the action
        std::string action = String::ToLower(args[0]);

        // check args based on action
        if (action == "add" || action == "remove") {
            if (args.size() < 2) {
                AddCommandMessage("&cUsage: .friend <add|remove> <name>");
                return;
            }
        } else if (action == "list" || action == "clear") {
            if (args.size() > 1) {
                AddCommandMessage("&cUsage: .friend <list|clear>");
                return;
            }
        } else {
            AddCommandMessage("&cInvalid action. Valid actions are: add, remove, list, clear");
            return;
        }


        // Check the action
        if (action == "add") {
            // Get the name
            std::string name = args[1];

            // Remove the @ if it exists
            if (name[0] == '@') {
                name = name.substr(1);
            }

            if (std::find(Prax::Preferences->Module.Friends.begin(), Prax::Preferences->Module.Friends.end(), name) != Prax::Preferences->Module.Friends.end()) {
                AddCommandMessage("&bYou are already friends with " + name);
                return;
            }

            AddCommandMessage("&aYou are now friends with " + name);
            Prax::Preferences->Module.Friends.push_back(name);
        } else if (action == "remove") {
            // Get the name
            std::string name = args[1];

            // Remove the @ if it exists
            if (name[0] == '@') {
                name = name.substr(1);
            }

            if (std::find(Prax::Preferences->Module.Friends.begin(), Prax::Preferences->Module.Friends.end(), name) != Prax ::Preferences->Module.Friends.end()) {
                Prax::Preferences->Module.Friends.erase(std::remove(Prax::Preferences->Module.Friends.begin(), Prax::Preferences->Module.Friends.end(), name), Prax::Preferences->Module.Friends.end());
                AddCommandMessage("&aYou are no longer friends with " + name);
            } else {
                AddCommandMessage("&cYou are not friends with " + name + "!");
                return;
            }
        } else if (action == "list") {
            std::string message = "Friends: ";
            // Check if the size is 0
            if (Prax::Preferences->Module.Friends.size() == 0) {
                AddCommandMessage("&cYou have no friends :(");
                return;
            }

            for (const auto &friendName: Prax::Preferences->Module.Friends) {
                message += friendName + ", ";
            }

            // Remove the last comma
            message = message.substr(0, message.size() - 2);

            AddCommandMessage(message);
            return;
        } else if (action == "clear") {
            Prax::Preferences->Module.Friends.clear();
            AddCommandMessage("&aCleared friends list");
            return;
        } else {
            AddCommandMessage("&cUsage: .friends <add|remove|list|clear> [name]");
            return;
        }
}
