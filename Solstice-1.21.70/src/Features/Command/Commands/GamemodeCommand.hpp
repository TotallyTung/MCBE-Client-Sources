#pragma once
//
// Created by vastrakai on 7/22/2024.
//

#include <Features/Command/Command.hpp>

class GamemodeCommand : public Command {
public:
    GamemodeCommand() : Command("gamemode") {}
    void execute(const std::vector<std::string>& args) override;
    [[nodiscard]] std::vector<std::string> getAliases() const override;
    [[nodiscard]] std::string getDescription() const override;
    [[nodiscard]] std::string getUsage() const override;
};