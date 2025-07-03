#pragma once

class HelpCommand : public Command
{
public:

    HelpCommand() : Command("help", "Get a list of commands", {"cmds",})
    {}

    bool execute(std::vector<std::string> cmd) override
    {
        ChatUtil::sendMessage(combine("Commands (", commandsPtr->size(), ")"));

        for (Command* cmd : *commandsPtr)
        {
            ChatUtil::sendPlainMessage(combine(DARK_GRAY, cmd->name, RESET, " - ", cmd->descr));
        }

        return false;
    }
};
