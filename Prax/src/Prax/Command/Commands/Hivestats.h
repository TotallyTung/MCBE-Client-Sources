//
// Created by vastrakai on 10/31/2023.
//


#pragma once

#include "Prax/Command/Command.h"

class Hivestats : public Command {
public:
    Hivestats() : Command("hivestats", "Displays stats about a player") {
        AddArgument("game", "The game to display stats for");
        AddArgument("player", "The player to display stats for");
    }

    void OnRun(std::vector<std::string> args) override;



};