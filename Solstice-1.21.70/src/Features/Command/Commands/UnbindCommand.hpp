#pragma once
//
// Created by vastrakai on 6/29/2024.
//

#include <Solstice.hpp>
#include <Features/FeatureManager.hpp>

class UnbindCommand : public Command {
public:
    UnbindCommand() : Command("unbind") {}
    void execute(const std::vector<std::string>& args) override;
    [[nodiscard]] std::vector<std::string> getAliases() const override;
    [[nodiscard]] std::string getDescription() const override;
    [[nodiscard]] std::string getUsage() const override;
};