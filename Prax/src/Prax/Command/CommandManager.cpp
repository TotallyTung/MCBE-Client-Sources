//
// Created by Flash on 7/30/2023.
//
#include "CommandManager.h"

#include <Prax/Utils/Misc.h>
#include <Prax/Utils/String.h>

#include <Prax/Command/Commands/Help.h>
#include <Prax/Command/Commands/TestCommand.h>
#include <Prax/Command/Commands/Bind.h>
#include <Prax/Command/Commands/Unbind.h>
#include <Prax/Command/Commands/Config.h>
#include <Prax/Command/Commands/Set.h>
#include <Prax/Command/Commands/Toggle.h>
#include <Prax/Command/Commands/Vclip.h>
#include <Prax/Command/Commands/CheckUser.h>
#include <Prax/Command/Commands/Binds.h>
#include <Prax/Command/Commands/FriendsCommand.h>
#include <Prax/Command/Commands/CheckAllUsers.h>
#include <Prax/Command/Commands/StreamerName.h>
#include <Prax/Command/Commands/Queue.h>
#include <Prax/Command/Commands/Hivestats.h>
#include <Prax/Command/Commands/Guiscale.h>
#include <Prax/Command/Commands/Eject.h>
#include <Prax/Command/Commands/Show.h>
#include <Prax/Command/Commands/TargetCommand.h>

#include "Commands/CheckAllStats.h"
#include "Commands/DamageTest.h"
#include "Commands/Hivekick.h"
#include "Commands/Panic.h"
#include "Commands/SevenDay.h"
#include "Commands/ClientName.h"
#include "Commands/HideCommand.h"


std::vector<Command*> CommandManager::Commands = std::vector<Command*>();

void CommandManager::Init()
{
    Commands.push_back(new Help());

    Commands.push_back(new Set());
    Commands.push_back(new Bind());
    Commands.push_back(new Vclip());
    Commands.push_back(new Binds());
    Commands.push_back(new Toggle());
    Commands.push_back(new Unbind());
    Commands.push_back(new Config());
    Commands.push_back(new CheckUser());
#ifndef __RELEASEFREE__
    Commands.push_back(new TestCommand());
    Commands.push_back(new Hivekick());
#endif
    Commands.push_back(new SevenDay());
    Commands.push_back(new StreamerName());
    Commands.push_back(new CheckAllUsers());
    Commands.push_back(new FriendsCommand());
    Commands.push_back(new Queue());
    Commands.push_back(new Hivestats());
    Commands.push_back(new Guiscale());
    Commands.push_back(new Eject());
    Commands.push_back(new DamageTest());
    Commands.push_back(new CheckAllStats());
    Commands.push_back(new Show());
    Commands.push_back(new HideCommand());
    Commands.push_back(new Panic());
    Commands.push_back(new ClientName());
    Commands.push_back(new TargetCommand());
}

void CommandManager::Shutdown()
{

}

void CommandManager::HandleInput(const std::string& originalInput, bool* cancelPacket) {

    // Copy the string so we can modify it
    auto input = originalInput;
    // Make sure it starts with a .

    if (input[0] != '.')
        return;

    const auto player = Minecraft::ClientInstance->ThePlayer();

    if (!player) return;

    *cancelPacket = true;

    // Remove the .
    input.erase(0, 1);

    // Split the input into a vector
    std::vector<std::string> args = String::SplitString(input, " ");

    // Get the command
    const std::string command = args[0];

    // Make sure the command exists
    for (const auto &cmd: Commands) {
        if (String::ToLower(cmd->Trigger) == String::ToLower(command)) {
            // Remove the command from the args
            args.erase(args.begin());

            // Make sure the amount of args match
            if (cmd->Arguments.size() == args.size() || cmd->Arguments.empty()) {
                // Run the command
                cmd->OnRun(args);
            } else {
                Logger::Write("Command", "Invalid amount of arguments for command: " + command, Logger::LogType::Error);
                Logger::Write("Command", "Usage: " + cmd->Usage, Logger::LogType::Error);



                player->addErrorMessage("Invalid amount of arguments for command: " + command);
                player->addChatMessage("Usage: " + cmd->Usage);
            }

            return;
            // Alias handling
        } else if (!cmd->Aliases.empty()) {
            for (auto &alias: cmd->Aliases) {
                if (String::ToLower(alias) == String::ToLower(command)) {
                    // Remove the command from the args
                    args.erase(args.begin());

                    // Make sure the amount of args match
                    if (cmd->Arguments.size() == args.size() || cmd->Arguments.empty()) {
                        // Run the command
                        cmd->OnRun(args);
                    } else {
                        Logger::Write("Command", "Invalid amount of arguments for command: " + command, Logger::LogType::Error);
                        Logger::Write("Command", "Usage: " + cmd->Usage, Logger::LogType::Error);

                        player->addErrorMessage("Invalid amount of arguments for command: " + command);
                        player->addChatMessage("Usage: " + cmd->Usage);
                    }

                    return;
                }
            }
        }
    }

    // Command doesn't exist
    Logger::Write("Command", "Command not found: " + command, Logger::LogType::Error);

    player->addErrorMessage("Command not found: " + command);
}
