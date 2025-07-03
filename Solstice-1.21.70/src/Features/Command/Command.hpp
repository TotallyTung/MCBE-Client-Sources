#pragma once
//
// Created by vastrakai on 6/28/2024.
//

#include <vector>
#include <string>

class Command {
public:
    std::string name;

    explicit Command(const std::string& name);

    virtual void execute(const std::vector<std::string>& args) = 0;
    [[nodiscard]] virtual std::vector<std::string> getAliases() const = 0;
    [[nodiscard]] virtual std::string getDescription() const = 0;
    [[nodiscard]] virtual std::string getUsage() const = 0;
    [[nodiscard]] bool matchName(std::string_view toMatch) const;
    [[nodiscard]] std::vector<std::string> getNames() const;
};