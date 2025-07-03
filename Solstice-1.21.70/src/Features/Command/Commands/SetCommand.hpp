#pragma once
//
// Created by vastrakai on 7/10/2024.
//

#include <Features/Command/Command.hpp>

class SetCommand : public Command {
public:
    SetCommand() : Command("set") {}
    void execute(const std::vector<std::string>& args) override;
    [[nodiscard]] std::vector<std::string> getAliases() const override;
    [[nodiscard]] std::string getDescription() const override;
    [[nodiscard]] std::string getUsage() const override;
};