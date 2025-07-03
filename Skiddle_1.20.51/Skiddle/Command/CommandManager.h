#pragma once

#include "Commands/HelpCommand.h"
#include "Commands/ConfigCommand.h"
#include "Commands/BindCommand.h"
#include "Commands/UnbindCommand.h"
#include "Commands/VClipCommand.h"
#include "Commands/EjectCommand.h"

class CommandManager
{
public:
	void InitCommands()
	{
		commandsPtr = &commands; 

        commands.push_back(new HelpCommand());
        commands.push_back(new ConfigCommand());
        commands.push_back(new BindCommand());
        commands.push_back(new UnbindCommand());
        commands.push_back(new VClipCommand());
        commands.push_back(new EjectCommand());

#if _DEBUG
        std::cout << "Initialized commands" << std::endl << std::endl;
#endif // _DEBUG
	}

	~CommandManager()
	{
		for (auto cmd : commands)
			delete cmd;
	}

    bool sendCommand(const std::string& command)
    {
        Command* cur = nullptr;

        if (command.length() < 2)
            return true;

        std::string argString = command.substr(1);
        std::string commandName = "";
        std::vector<std::string> arguments = turnToVec(argString, commandName);

        for (auto cmd : commands)
        {
            if (cmd->name == commandName)
            {
                cur = cmd;
                break;
            }

            for (const auto& alias : cmd->aliases)
            {
                if (alias == commandName)
                {
                    cur = cmd;
                    break;
                }
            }
        }

        if (!cur)
        {
            // ChatUtil::sendMessage(xorstr_("Invalid Command!"));
            ChatUtil::sendMessage(combine("Command: ", GRAY, argString, RESET, " is not command, do '.help'"));
            return true;
        }

        cur->execute(arguments);

        return true;
    }

    std::vector<Command*> commands;

private:
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
            std::transform(commandName.begin(), commandName.end(), commandName.begin(), tolower);
            ret.erase(ret.begin());
        }

        return ret;
    }
};