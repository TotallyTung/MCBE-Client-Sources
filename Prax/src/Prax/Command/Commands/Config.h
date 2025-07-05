//
// Created by Flash on 8/23/2023.
//

#pragma once

#include "../Command.h"

class Config : public Command {
public:
    Config() : Command("Config", "Loads or saves configs", "", {"c"}) {
        // AddArgument("option", "Whether to load or save the config");
        // AddArgument("name", "The name of the config");
    }

    void ShowUsage();
    void OnRun(std::vector<std::string> args) override;
};
