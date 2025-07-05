//
// Created by Flash on 7/30/2023.
//

#pragma once

#include "../Command.h"

class TestCommand : public Command {
public:
    TestCommand() : Command("test", "test", "test") {
    }

    void OnRun(std::vector<std::string> args) override;
};