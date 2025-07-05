//
// Created by vastrakai on 1/28/2024.
//


#pragma once
#include <Prax/Command/Command.h>

class SevenDay : public Command {
public:
    SevenDay() : Command("SevenDay", "Seven days yourself from The Hive") {}

    static std::thread thread;
    void OnRun(std::vector<std::string> args) override;
};
