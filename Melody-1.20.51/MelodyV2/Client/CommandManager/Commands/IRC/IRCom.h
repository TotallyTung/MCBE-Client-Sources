// Created by Tony on 2024-10-10 10:01:54

// IRCOMMAND.h
#pragma once

#include "../Command.h"

class IRCOMMAND : public Command {
public:
    IRCOMMAND();
    void execute(const std::vector<std::string>& args) override;
};