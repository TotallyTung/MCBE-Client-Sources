#pragma once

#include "Commands/BindCommand.h"
#include "Commands/ConfigurationCommand.h"

#include <memory>
#include <vector>
#include <string>
#include <algorithm>

class CommandManager
{
public:
    CommandManager() {
        InitializeCommands();
    }

    ~CommandManager() = default; // std::unique_ptr automatically handles memory cleanup

    void InitializeCommands()
    {
        // Initialize commands with unique_ptr
        mCommands.push_back(std::make_unique<BindCommand>());
        mCommands.push_back(std::make_unique<ConfigurationCommand>());
    }

    bool sendCommand(const std::string& command)
    {
        if (command.length() < 2)
            return true;

        std::string argString = command.substr(1);
        std::string commandName;
        std::vector<std::string> arguments = turnToVec(argString, commandName);

        for (const auto& cmd : mCommands)
        {
            if (cmd->mName == commandName)
            {
                cmd->execute(arguments);
                return true;
            }

            for (const auto& alias : cmd->mAliases)
            {
                if (alias == commandName)
                {
                    cmd->execute(arguments);
                    return true;
                }
            }
        }

        ChatUtil::sendMessage(Util::combine("Command '", TextFormat::getColorCode(TextFormat::Color::GRAY), command, TextFormat::getStyleCode(TextFormat::Style::RESET), "' could not be found!"));
        return true;
    }

private:
    std::vector<std::unique_ptr<Command>> mCommands;

    std::vector<std::string> turnToVec(std::string str, std::string& commandName)
    {
        std::vector<std::string> ret;
        size_t pos = 0;
        bool inQuotes = false;

        while (pos < str.length())
        {
            if (str[pos] == '"')
            {
                inQuotes = !inQuotes;
                pos++;
                continue;
            }

            if (!inQuotes && str[pos] == ' ')
            {
                pos++;
                continue;
            }

            size_t start = pos;
            if (inQuotes)
            {
                pos = str.find('"', pos);
                if (pos == std::string::npos)
                    break;
            }
            else
            {
                pos = str.find(' ', pos);
                if (pos == std::string::npos)
                    pos = str.length();
            }

            std::string arg = str.substr(start, pos - start);
            ret.push_back(arg);
            pos++;
        }

        if (ret.size())
        {
            commandName = ret[0];
            std::transform(commandName.begin(), commandName.end(), commandName.begin(), ::tolower);
            ret.erase(ret.begin());
        }

        return ret;
    }
};