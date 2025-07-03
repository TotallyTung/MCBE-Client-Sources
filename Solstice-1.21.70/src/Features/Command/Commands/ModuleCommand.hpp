#pragma once
//
// Created by vastrakai on 7/12/2024.
//

#include <Features/Command/Command.hpp>

class ModuleCommand : public Command {
public:
    ModuleCommand() : Command("module") {}
    void execute(const std::vector<std::string>& args) override;
    [[nodiscard]] std::vector<std::string> getAliases() const override;
    [[nodiscard]] std::string getDescription() const override;
    [[nodiscard]] std::string getUsage() const override;
};