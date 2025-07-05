#pragma once
//
// Created by vastrakai on 9/7/2023.
//


#include <Prax/Command/Command.h>

class CheckUser : public Command {
public:
    CheckUser() : Command("CheckUser", "Checks if the given username is a valid xbox live account") {
        AddArgument("username", "Username to check");
    }

    void OnRun(std::vector<std::string> args) override;
};