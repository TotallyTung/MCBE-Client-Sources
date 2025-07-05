//
// Created by Flash on 7/30/2023.
//

#pragma once

#include <string>
#include "../Command.h"

class Bind : public Command {
public:
    Bind() : Command("Bind", "Binds a module to a key.", "", { "b" }) {
        AddArgument("module", "The module to bind.");
        AddArgument("key", "The key to bind the module to.");
    }

    void OnRun(std::vector<std::string> args) override;
};