#pragma once
//
// Created by vastrakai on 8/24/2024.
//


class IrcCommand : public Command
{
public:
    IrcCommand() : Command("irc") {}

    void execute(const std::vector<std::string>& args) override;
    [[nodiscard]] std::vector<std::string> getAliases() const override;
    [[nodiscard]] std::string getDescription() const override;
    [[nodiscard]] std::string getUsage() const override;
};