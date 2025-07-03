//
// Created by vastrakai on 6/28/2024.
//

#include "TestCommand.hpp"

#include <Utils/GameUtils/ChatUtils.hpp>
#include <magic_enum.hpp>
#include <Features/IRC/WorkingVM.hpp>
#include <Hook/Hooks/RenderHooks/D3DHook.hpp>

void TestCommand::execute(const std::vector<std::string>& args)
{
    ChatUtils::displayClientMessage("Test command executed!");
    // Display the amount of Modules and Commands
    ChatUtils::displayClientMessage(std::to_string(gFeatureManager->mModuleManager->mModules.size()) + " modules and "
        + std::to_string(gFeatureManager->mCommandManager->mCommands.size()) + " commands are currently loaded.");

    // get the first argument, if any
    if (args.size() <= 1)
    {
        ChatUtils::displayClientMessage("Available subcommands: showconsole, fallback, setloglevel[log level], enforcedebug, crash, exec");
        return;
    }

    const std::string& arg = args[1];

    if (arg == "exec")
    {
        // Use the next argument as the script name
        if (args.size() < 3)
        {
            ChatUtils::displayClientMessage("Usage: .test exec [script name]");
            return;
        }

        const std::string& scriptName = args[2];
        // Gather arguments for the script
        std::vector<std::string> scriptArgs;
        for (size_t i = 3; i < args.size(); i++)
        {
            scriptArgs.push_back(args[i]);
        }

        bool execSuccess = gFeatureManager->mScriptManager->findAndExecCommand(scriptName, scriptArgs);
        if (!execSuccess)
        {
            ChatUtils::displayClientMessage("§cThe command " + scriptName + " was not found!");
        }
    }

    if (arg == "showconsole")
    {
        Logger::initialize();
        ChatUtils::displayClientMessage("Console initialized!");
        return;
    }

    if (arg == "crash")
    {
        // Crash the game
        int* crash = nullptr;
        *crash = 0;
        return;
    }

    if (arg == "bruh")
    {
        BlockPos a = { 1, 2, 3 };
        BlockPos b = { 4, 5, 6 };
        glm::ivec3 c = b - a;
        glm::ivec3 expected = { 3, 3, 3 };
        ChatUtils::displayClientMessage("Subtraction result: " + std::to_string(c.x) + ", " + std::to_string(c.y) + ", " + std::to_string(c.z));
        ChatUtils::displayClientMessage("Expected result: " + std::to_string(expected.x) + ", " + std::to_string(expected.y) + ", " + std::to_string(expected.z));
        return;
    }

    if (arg == "fallback")
    {
        D3DHook::forceFallback = true;
        ChatUtils::displayClientMessage("Attempting to force fallback to D3D11!");
        return;
    }

    // spdlog::level::trace
    if (arg == "setloglevel")
    {
        if (args.size() < 3)
        {
            ChatUtils::displayClientMessage("Usage: .test setloglevel [log level]");
            return;
        }

        const std::string& arg2 = args[2];

        for (std::string_view level : magic_enum::enum_names<spdlog::level::level_enum>())
        {
            spdlog::info("Checking level {}", level);
            if (level == arg2)
            {
                spdlog::info("Setting log level to {}", level);
                spdlog::set_level(spdlog::level::from_str(level.data()));
                ChatUtils::displayClientMessage("Log level set to " + std::string(level));
                return;
            }
        }
        return;
    }

    if (arg == "enforcedebug")
    {
#ifdef __DEBUG__
        Solstice::Prefs->mEnforceDebugging = !Solstice::Prefs->mEnforceDebugging;
        PreferenceManager::save(Solstice::Prefs);
        ChatUtils::displayClientMessage("Enforce debugging set to {}", Solstice::Prefs->mEnforceDebugging ? "true" : "false");
        ChatUtils::displayClientMessage("You may need to reinject for this to take effect.");
#else
        ChatUtils::displayClientMessage("This command is only available in debug builds.");
#endif
    }
}

std::vector<std::string> TestCommand::getAliases() const
{
    return {};
}

std::string TestCommand::getDescription() const
{
    return "A command for testing purposes.";
}

std::string TestCommand::getUsage() const
{
    return "Usage: .test";
}
