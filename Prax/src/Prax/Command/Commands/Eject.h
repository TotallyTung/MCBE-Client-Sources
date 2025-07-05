//
// Created by vastrakai on 12/28/2023.
//


#pragma once

#include <Prax/Command/Command.h>

class Eject : public Command {
public:
    Eject() : Command("Eject", "Ejects Prax", "", { "uninject" }) {
    }

    void OnRun(std::vector<std::string> args) override;
};