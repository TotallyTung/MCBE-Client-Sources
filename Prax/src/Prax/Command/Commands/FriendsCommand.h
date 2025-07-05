#pragma once
//
// Created by vastrakai on 9/22/2023.
//


#include <Prax/Command/Command.h>

class FriendsCommand : public Command {
public:
    FriendsCommand() : Command("friend", "Friends command") {
    }

    void OnRun(std::vector<std::string> args) override;

};