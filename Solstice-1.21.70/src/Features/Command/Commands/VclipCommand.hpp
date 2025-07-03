//
// Created by alteik on 03/09/2024.
//
#include <Solstice.hpp>
#include <Features/FeatureManager.hpp>
#include <Features/Command/Command.hpp>
#include <Features/Command/CommandManager.hpp>

#include "spdlog/spdlog.h"

class VclipCommand : public Command
{
public:
    VclipCommand() : Command("vclip") {}
    void execute(const std::vector<std::string>& args) override;
    [[nodiscard]] std::vector<std::string> getAliases() const override;
    [[nodiscard]] std::string getDescription() const override;
    [[nodiscard]] std::string getUsage() const override;
    [[nodiscard]] bool isNumber(std::string str);
};