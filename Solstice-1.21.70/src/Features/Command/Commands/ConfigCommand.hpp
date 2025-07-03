#pragma once
//
// Created by vastrakai on 7/2/2024.
//

#include <Solstice.hpp>
#include <Features/FeatureManager.hpp>
#include "spdlog/spdlog.h"

class ConfigCommand : public Command {
public:
    ConfigCommand() : Command("config") {}
    void execute(const std::vector<std::string>& args) override;
    [[nodiscard]] std::vector<std::string> getAliases() const override;
    [[nodiscard]] std::string getDescription() const override;
    [[nodiscard]] std::string getUsage() const override;
};