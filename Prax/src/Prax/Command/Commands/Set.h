//
// Created by Flash on 7/30/2023.
//

#pragma once

#include "../Command.h"

class Set : public Command {
public:


    Set() : Command("Set", "Allows you to set a value of a setting") {
        AddArgument("Module", "The module in which the target setting lies");
        AddArgument("Setting", "The setting you want to change");
        AddArgument("Value", "The value you want to set the setting to");
    }

    void OnRun(std::vector<std::string> args) override;
};