#pragma once
//
// Created by vastrakai on 9/25/2023.
//

#include <Prax/Command/Command.h>

class StreamerName : public Command {
public:
    StreamerName() : Command("StreamerName", "Sets the name for StreamerMode") {
        AddArgument("username", "Username to set");
    }

    void OnRun(std::vector<std::string> args) override;
};