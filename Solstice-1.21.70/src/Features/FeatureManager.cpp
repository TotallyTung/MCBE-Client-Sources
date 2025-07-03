//
// Created by vastrakai on 6/28/2024.
//

#include "FeatureManager.hpp"

#include "Command/CommandManager.hpp"
#include "Modules/ModuleManager.hpp"

void FeatureManager::init()
{
    spdlog::info("initializing FeatureManager");

    spdlog::info("initializing event dispatcher");
    mDispatcher = std::make_unique<nes::event_dispatcher>();
    spdlog::info("Successfully created event dispatcher");

    spdlog::info("initializing ModuleManager");
    mModuleManager = std::make_shared<ModuleManager>();
    mModuleManager->init();
    spdlog::info("Successfully initialized " + std::to_string(mModuleManager->mModules.size()) + " modules");

    spdlog::info("initializing CommandManager");
    mCommandManager = std::make_shared<CommandManager>();
    mCommandManager->init();
    spdlog::info("Successfully initialized " + std::to_string(mCommandManager->mCommands.size()) + " commands");

    spdlog::info("initializing ScriptManager");
    mScriptManager = std::make_shared<ScriptManager>();
    spdlog::info("Successfully initialized ScriptManager");

    spdlog::info("FeatureManager initialized");
}

void FeatureManager::shutdown()
{
    spdlog::info("shutting down FeatureManager");

    spdlog::info("shutting down CommandManager");
    mCommandManager->shutdown();

    spdlog::info("shutting down ModuleManager");
    mModuleManager->shutdown();

    spdlog::info("shutting down event dispatcher");
    mDispatcher.reset();
}