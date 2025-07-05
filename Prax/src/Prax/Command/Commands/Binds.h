//
// Created by Flash on 9/16/2023.
//

#pragma once

#include <string>
#include "../Command.h"

class Binds : public Command {
public:
    Binds() : Command("Binds", "Lists module binds") {
    }

    void OnRun(std::vector<std::string> args) override;
};