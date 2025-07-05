//
// Created by vastrakai on 9/25/2023.
//

#include <Prax/Module/Modules/Misc/StreamerMode.h>
#include "StreamerName.h"

void StreamerName::OnRun(std::vector<std::string> args) {
    if (args.size() < 1) {
        AddCommandMessage("Usage: .StreamerName <username>");
        return;
    }

    // Combine the args into a single string separated by spaces
    std::string username = "";
    for (int i = 0; i < args.size(); i++) {
        username += args[i];
        if (i != args.size() - 1) {
            username += " ";
        }
    }

    // Set the streamer name
    StreamerMode::NewName = username;
    Prax::Preferences->Module.StreamerName = username;
}
