//
// Created by Flash on 9/9/2023.
//

#pragma once

#include "../Command.h"
#include <Prax/Command/CommandManager.h>

class Help : public Command {
public:
    Help() : Command("Help", "Shows you all the commands") {
    }

    void OnRun(std::vector<std::string> args) override;
};