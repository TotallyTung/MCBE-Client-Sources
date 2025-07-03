//
// Created by vastrakai on 6/28/2024.
//

#include "CommandManager.hpp"

#include <sstream>
#include <Features/FeatureManager.hpp>
#include <Utils/GameUtils/ChatUtils.hpp>
#include <Utils/StringUtils.hpp>

#include "Commands/BindCommand.hpp"
#include "Commands/BuildInfoCommand.hpp"
#include "Commands/ConfigCommand.hpp"
#include "Commands/CopyNameCommand.hpp"
#include "Commands/FlingCommand.hpp"
#include "Commands/FriendCommand.hpp"
#include "Commands/HelpCommand.hpp"
#include "Commands/QueueCommand.hpp"
#include "Commands/SetCommand.hpp"
#include "Commands/TestCommand.hpp"
#include "Commands/ToggleCommand.hpp"
#include "Commands/UnbindCommand.hpp"
#include "Commands/ModuleCommand.hpp"
#include "Commands/GamemodeCommand.hpp"
#include "Commands/HiveStatsCommand.hpp"
#include "Commands/IrcCommand.hpp"
#include "Commands/LuaCommand.hpp"
#include "Commands/VclipCommand.hpp"
#include "Commands/SnipeCommand.hpp"
#include "Commands/NameProtectCommand.hpp"
#include "Commands/TeleportCommand.hpp"
#include "Features/Events/ChatEvent.hpp"
#include "spdlog/spdlog.h"

void CommandManager::init()
{
#ifdef __DEBUG__
    ADD_COMMAND(TestCommand);
#endif

    ADD_COMMAND(HelpCommand);
    ADD_COMMAND(ToggleCommand);
    ADD_COMMAND(BindCommand);
    ADD_COMMAND(UnbindCommand);
    ADD_COMMAND(ConfigCommand);
    ADD_COMMAND(CopyNameCommand);
    ADD_COMMAND(QueueCommand);
    ADD_COMMAND(SetCommand);
    ADD_COMMAND(ModuleCommand);
    ADD_COMMAND(FriendCommand);
    ADD_COMMAND(GamemodeCommand);
    ADD_COMMAND(BuildInfoCommand);
    ADD_COMMAND(HiveStatsCommand);
    ADD_COMMAND(IrcCommand);
    ADD_COMMAND(FlingCommand);
    ADD_COMMAND(VclipCommand);
    ADD_COMMAND(SnipeCommand);
    ADD_COMMAND(NameProtectCommand);
    ADD_COMMAND(TeleportCommand);

    ADD_COMMAND(LuaCommand);

    // Look for any commands that have duplicate names
    for (size_t i = 0; i < mCommands.size(); i++)
    {
        for (size_t j = i + 1; j < mCommands.size(); j++)
        {
            auto names1 = mCommands[i]->getNames();
            auto names2 = mCommands[j]->getNames();

            for (const auto& name1 : names1)
            {
                for (const auto& name2 : names2)
                {
                    if (name1 == name2)
                    {
                        spdlog::error("Commands [{}] and [{}] have the same name: {}", names1[0], names2[0], name1);
                        throw std::runtime_error("Duplicate command names: " + std::string(name1));
                    }
                }
            }
        }
    }

    gFeatureManager->mDispatcher->listen<ChatEvent, &CommandManager::handleCommand>(this);
}

void CommandManager::shutdown()
{
    gFeatureManager->mDispatcher->deafen<ChatEvent, &CommandManager::handleCommand>(this);
    mCommands.clear();

    spdlog::info("Successfully shut down CommandManager");
}

void CommandManager::handleCommand(ChatEvent& event)
{
    std::string command = event.getMessage();

    if (command[0] != '.')
        return;

    event.setCancelled(true);

    // Remove the dot from the comand
    std::string cmd = command.substr(1);

    // Split the command into arguments
    const std::vector<std::string> args = StringUtils::split(cmd, ' ');

    if (args.empty())
        return;

    // Find the command
    const std::string_view commandName = args[0];

    auto it = std::ranges::find_if(mCommands, [=](const auto& command)
    {
        return command->matchName(commandName);
    });

    if (it == mCommands.end())
    {
        bool found = gFeatureManager->mScriptManager->findAndExecCommand(std::string(commandName), args);
        if (!found && !event.mSpecial) ChatUtils::displayClientMessage("§cThe command " + std::string(commandName) + " was not found!");
        return;
    }

    // Execute the command
    spdlog::info("Executing command: {}", commandName);
    (*it)->execute(args);
}

std::vector<Command*> CommandManager::getCommands() const
{
    std::vector<Command*> commands;
    commands.reserve(mCommands.size());

    for (const auto& command : mCommands)
    {
        commands.push_back(command.get());
    }

    return commands;
}
