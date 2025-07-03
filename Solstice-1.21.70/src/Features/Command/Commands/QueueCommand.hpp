#pragma once
//
// Created by vastrakai on 7/8/2024.
//
#include <Features/FeatureManager.hpp>

class QueueCommand : public Command {
public:
    QueueCommand() : Command("queue") {}
    void execute(const std::vector<std::string>& args) override;
    [[nodiscard]] std::vector<std::string> getAliases() const override;
    [[nodiscard]] std::string getDescription() const override;
    [[nodiscard]] std::string getUsage() const override;
};