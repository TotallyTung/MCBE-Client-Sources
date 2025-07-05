//
// Created by vastrakai on 4/13/2024.
//


#pragma once

#include <Prax/Command/Command.h>

class TargetCommand : public Command {
public:
    TargetCommand() : Command("target", "Target command for AutoSnipe") {
    }

    void OnRun(std::vector<std::string> args) override;

};