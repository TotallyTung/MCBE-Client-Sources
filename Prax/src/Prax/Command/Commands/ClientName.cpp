//
// Created by vastrakai on 4/1/2024.
//

#include "ClientName.h"

void ClientName::ShowUsage() {
    AddCommandMessage("Changes the name of the client", Prax::ClientName);
    AddCommandMessage(".ClientName <name>", Prax::ClientName);
}

void ClientName::OnRun(std::vector<std::string> args) {
    if (args.empty()) {
        ShowUsage();
        return;
    }

    // join the arguments into a single string
    std::string name = String::Join(args, " ");

    Prax::ClientName = name;
    Prax::Preferences->ClientName = name;

    AddCommandMessage("&aChanged client name to &b" + name, Prax::ClientName);
}
