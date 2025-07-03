#pragma once
//
// Created by vastrakai on 9/23/2024.
//


class LuaCommand : public Command
{
public:
    LuaCommand() : Command("lua") {}

    void execute(const std::vector<std::string>& args) override;
    [[nodiscard]] std::vector<std::string> getAliases() const override;
    [[nodiscard]] std::string getDescription() const override;
    [[nodiscard]] std::string getUsage() const override;
};