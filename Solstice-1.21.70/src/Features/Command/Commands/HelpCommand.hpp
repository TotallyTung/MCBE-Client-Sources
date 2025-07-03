#pragma once
#include <Solstice.hpp>
#include <Features/FeatureManager.hpp>
#include <Features/Command/Command.hpp>
#include <Features/Command/CommandManager.hpp>

#include "spdlog/spdlog.h"
//
// Created by vastrakai on 6/29/2024.
//



class HelpCommand : public Command {
public:
    HelpCommand() : Command("help") {}
    void execute(const std::vector<std::string>& args) override;
    [[nodiscard]] std::vector<std::string> getAliases() const override;
    [[nodiscard]] std::string getDescription() const override;
    [[nodiscard]] std::string getUsage() const override;
};