#pragma once
//
// Created by vastrakai on 9/24/2023.
//


#include <Prax/Command/Command.h>

class CheckAllUsers : public Command {
public:
    CheckAllUsers() : Command("CheckAllUsers", "Check all users") {
    }

    void OnRun(std::vector<std::string> args) override;

};