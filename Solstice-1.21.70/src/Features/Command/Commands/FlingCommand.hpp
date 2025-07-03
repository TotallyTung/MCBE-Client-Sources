#pragma once
//
// Created by vastrakai on 8/30/2024.
//

#include <Features/Command/Command.hpp>

class FlingCommand : public Command {
public:
    FlingCommand() : Command("fling") {}
    void execute(const std::vector<std::string>& args) override;
    [[nodiscard]] std::vector<std::string> getAliases() const override;
    [[nodiscard]] std::string getDescription() const override;
    [[nodiscard]] std::string getUsage() const override;
};