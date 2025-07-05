//
// Created by Flash on 7/30/2023.
//

#pragma once

#include <string>
#include "../Command.h"

class Unbind : public Command {
public:
    Unbind() : Command("Unbind", "Unbinds a module to a key.") {
        AddArgument("module", "The module to unbind.");
    }

    void OnRun(std::vector<std::string> args) override;
};