#pragma once
//
// Created by vastrakai on 8/17/2024.
//
#include <Features/Command/Command.hpp>

class HiveStatsCommand : public Command
{
public:
    HiveStatsCommand() : Command("hivestats") {}

    std::unique_ptr<HttpRequest> mRequest;
    bool mIsRequesting = false;

    static void onStatsReceived(HttpResponseEvent event);
    void execute(const std::vector<std::string>& args) override;
    [[nodiscard]] std::vector<std::string> getAliases() const override;
    [[nodiscard]] std::string getDescription() const override;
    [[nodiscard]] std::string getUsage() const override;
};