//
// Created by Flash on 7/30/2023.
//

#pragma once

#include <map>
#include <vector>
#include <src/Prax/SDK/Game/Minecraft.h>


class Command {
public:
    virtual ~Command() = default;

    std::string Trigger;
    std::string Description;
    std::string Usage;
    std::vector<std::string> Aliases = {};

    // name and type
    std::map<std::string, std::string> Arguments;

    Command(const std::string& trigger, const std::string& description, const std::string& usage = "", const std::vector<std::string>& aliases = {});

    virtual void OnRun(std::vector<std::string> args) = 0;
    void AddArgument(const std::string& name, const std::string& type);
    void CreateUsage();
    void ShowUsage(const std::function<void()>& callback = nullptr);
    void AddCommandMessage(std::string message, std::string sender = "Prax") const;
};