#pragma once
//
// Created by alteik on 13/10/2024.
//

class NameProtectCommand : public Command
{
public:
    NameProtectCommand() : Command("setname") {}
    void execute(const std::vector<std::string>& args) override;
    [[nodiscard]] std::vector<std::string> getAliases() const override;
    [[nodiscard]] std::string getDescription() const override;
    [[nodiscard]] std::string getUsage() const override;
};