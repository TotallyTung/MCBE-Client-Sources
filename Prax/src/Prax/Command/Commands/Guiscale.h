//
// Created by vastrakai on 12/25/2023.
//


#pragma once
#include <Prax/Command/Command.h>

class Guiscale : public Command {
public:
    Guiscale() : Command("Guiscale", "Sets the gui scale") {
        AddArgument("scale", "Scale to set");
    }

    void OnRun(std::vector<std::string> args) override;
};
