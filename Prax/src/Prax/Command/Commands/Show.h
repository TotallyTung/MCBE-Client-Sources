//
// Created by Flash on 1/2/2024.
//


#pragma once

#include "../Command.h"

class Show  : public Command {
public:
    Show() : Command("Show", "Allows you to show a module in the arraylist") {
        AddArgument("Module", "The module you want to show");
    }

    void OnRun(std::vector<std::string> args) override;
};