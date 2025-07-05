//
// Created by vastrakai on 1/1/2024.
//


#pragma once
#include <Prax/Command/Command.h>

class CheckAllStats : public Command {
public:
    CheckAllStats() : Command("CheckAllStats", "Check all stats", "", { "cas", "hvh" }) {}

    static void Exec();

    void OnRun(std::vector<std::string> args) override;

};
