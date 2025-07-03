#pragma once
//
// Created by vastrakai on 11/9/2024.
//
#include <Features/Command/Command.hpp>

class TeleportCommand : public Command
{
public:
    TeleportCommand() : Command("teleport") {}
    void execute(const std::vector<std::string>& args) override;
    [[nodiscard]] std::vector<std::string> getAliases() const override;
    [[nodiscard]] std::string getDescription() const override;
    [[nodiscard]] std::string getUsage() const override;
};