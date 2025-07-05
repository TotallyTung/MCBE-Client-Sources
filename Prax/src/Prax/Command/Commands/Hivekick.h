//
// Created by vastrakai on 1/6/2024.
//


#pragma once

#include "../Command.h"

class Hivekick : public Command {
public:
    Hivekick() : Command("Hivekick", "Kicks yourself from The Hive") {}

    static std::thread thread;
    void OnRun(std::vector<std::string> args) override;
};