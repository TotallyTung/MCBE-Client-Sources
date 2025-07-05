//
// Created by vastrakai on 5/1/2024.
//


#pragma once

#include <Prax/Command/Command.h>

class HideCommand : public Command {
public:
    HideCommand() : Command("Hide", "Hides/shows a module in the arraylist") {
        AddArgument("module", "The name of the module to hide/show");
    }

    void OnRun(std::vector<std::string> args) override;
};