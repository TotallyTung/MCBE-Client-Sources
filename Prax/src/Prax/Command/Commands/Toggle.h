//
// Created by Flash on 7/30/2023.
//

#pragma once

#include "../Command.h"

class Toggle : public Command {
public:
    Toggle() : Command("Toggle", "Toggles modules", "", { "t" }) {
        AddArgument("module", "Module to toggle");
    }

    void OnRun(std::vector<std::string> args) override;
};