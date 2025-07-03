//
// Created by alteik on 03/09/2024.
//

#include <Features/FeatureManager.hpp>

class SnipeCommand : public Command {
public:
    SnipeCommand() : Command("snipe") {}
    void execute(const std::vector<std::string>& args) override;
    [[nodiscard]] std::vector<std::string> getAliases() const override;
    [[nodiscard]] std::string getDescription() const override;
    [[nodiscard]] std::string getUsage() const override;
};