//
// Created by vastrakai on 9/23/2024.
//

#include "LuaCommand.hpp"

void LuaCommand::execute(const std::vector<std::string>& args)
{
    if (args.size() < 2)
    {
        ChatUtils::displayClientMessage("§cInvalid arguments! Usage: {}", getUsage());
        return;
    }

    if (args[1] == "hotreload")
    {
        gFeatureManager->mScriptManager->mRunning = !gFeatureManager->mScriptManager->mRunning;
        if (!gFeatureManager->mScriptManager->mRunning)
        {
            ChatUtils::displayClientMessage("§cHot-reloading is now disabled.");
            ChatUtils::displayClientMessage("§eScripts will not be reloaded automatically.");
            return;
        }
        ChatUtils::displayClientMessage("§aHot-reloading is now enabled.");
        ChatUtils::displayClientMessage("§aScripts will be reloaded automatically when they are modified.");
    }
    else if (args[1] == "reload")
    {
        gFeatureManager->mScriptManager->unloadUserScripts();
        gFeatureManager->mScriptManager->loadUserScripts();
        ChatUtils::displayClientMessage("§aReloaded Lua scripts.");
    }
    else if (args[1] == "exec")
    {
        if (args.size() < 3)
        {
            ChatUtils::displayClientMessage("§cInvalid arguments! Usage: {}", getUsage());
            return;
        }

        std::string script = args[2];
        std::vector<std::string> scriptArgs;
        for (size_t i = 3; i < args.size(); i++)
        {
            scriptArgs.push_back(args[i]);
        }
        bool s = gFeatureManager->mScriptManager->findAndExecCommand(script, scriptArgs);
        if (!s) ChatUtils::displayClientMessage("§cThe script " + script + " was not found!");

    }
    else if (args[1] == "test")
    {
        gFeatureManager->mScriptManager->execTests();
    }
    else
    {
        ChatUtils::displayClientMessage("§cInvalid arguments! Usage: {}", getUsage());
    }
}

std::vector<std::string> LuaCommand::getAliases() const
{
    return {};
}

std::string LuaCommand::getDescription() const
{
    return "Manage Lua scripts.";
}

std::string LuaCommand::getUsage() const
{
    return ".lua <hotreload|reload|exec|test> [script]";
}
