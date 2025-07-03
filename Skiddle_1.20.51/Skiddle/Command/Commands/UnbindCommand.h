#pragma once

class UnbindCommand : public Command
{
public:

    UnbindCommand() : Command("unbind", "Unbind a module to a key", {})
    {}

    bool execute(std::vector<std::string> cmd) override // bro has no error handling with chat messages imagineeee
    {
        if (cmd.size() < 1)
            return false;


        if (cmd[0] == "all")
        {
            for (auto module : modules)
            {
                module->setKeybind(0);
            }
            ChatUtil::sendMessage("Successfully unbound all!");
            return true;
        }

        auto mod = getModuleByName(cmd[0]);
        if (mod == nullptr)
            return false;

        if (cmd[0] == "all")
        {
            for (auto module : modules)
            {
                module->setKeybind(0);
            }
            ChatUtil::sendMessage("Successfully unbound all!");
            return true;
        }

        mod->setKeybind(0);
        ChatUtil::sendMessage(combine(BOLD, mod->getName(), RESET, " is now bound to NONE"));
    }
};
