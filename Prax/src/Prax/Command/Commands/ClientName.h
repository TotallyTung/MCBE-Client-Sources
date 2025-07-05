//
// Created by vastrakai on 4/1/2024.
//


#pragma once

#include <Prax/Command/Command.h>

class ClientName : public Command {
public:
    ClientName() : Command("clientname", "Changes the name of the client", "", {"cn"}) {
    }

    void ShowUsage();
    void OnRun(std::vector<std::string> args) override;
};