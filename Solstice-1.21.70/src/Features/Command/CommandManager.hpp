#pragma once
#include <future>
#include <memory>
#include <vector>

#include "Command.hpp"
//
// Created by vastrakai on 6/28/2024.
//


class CommandManager {
public:
    std::vector<std::unique_ptr<Command>> mCommands;

    void init();
    void shutdown();
    void handleCommand(class ChatEvent& event);
    std::vector<Command*> getCommands() const;
};

// Macro for command registration
#define ADD_COMMAND(COMMAND_CLASS) \
    gFeatureManager->mCommandManager->mCommands.emplace_back(std::make_unique<COMMAND_CLASS>()); \
    spdlog::info("Registered command: {}", #COMMAND_CLASS);