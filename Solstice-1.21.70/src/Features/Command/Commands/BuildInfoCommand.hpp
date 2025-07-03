#pragma once
//
// Created by vastrakai on 7/29/2024.
//

//static uint64_t DISCORD_USER_ID = 0x1F2F3F4F5F6F7F8F; // replaced at fetch time

// struct IdHolder
// {
//     uint64_t id;
//     explicit IdHolder(const uint64_t id) : id(id) {}
// };

class BuildInfoCommand : public Command {
public:
    BuildInfoCommand() : Command("buildinfo") {}
    void execute(const std::vector<std::string>& args) override;
    [[nodiscard]] std::vector<std::string> getAliases() const override;
    [[nodiscard]] std::string getDescription() const override;
    [[nodiscard]] std::string getUsage() const override;
};