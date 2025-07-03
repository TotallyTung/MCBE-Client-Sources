//
// Created by ssi on 10/20/2024.
//

#pragma once
#include <Features/Command/Command.hpp>

class CopyNameCommand : public Command {
public:
    CopyNameCommand() : Command("copyname") {}
    void execute(const std::vector<std::string>& args) override;
    [[nodiscard]] std::vector<std::string> getAliases() const override;
    [[nodiscard]] std::string getDescription() const override;
    [[nodiscard]] std::string getUsage() const override;
};