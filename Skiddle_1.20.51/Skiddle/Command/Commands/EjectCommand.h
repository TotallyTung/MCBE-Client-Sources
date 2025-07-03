#pragma once

class EjectCommand : public Command
{
public:

    EjectCommand() : Command("eject", "eject the client", {
        "uninject"
        })
    {}

    bool execute(std::vector<std::string> cmd) override
    {
        g_isRunning = false;
        return false;
    }
};
