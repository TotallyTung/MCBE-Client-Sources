//
// Created by vastrakai on 1/27/2024.
//


#pragma once
#include <Prax/Command/Command.h>

class Command;

class Panic : public Command {
public:
    Panic() : Command("Panic", "AHHH PANIC PANIC PANIC") {}

    void OnRun(std::vector<std::string> args) override;
};
