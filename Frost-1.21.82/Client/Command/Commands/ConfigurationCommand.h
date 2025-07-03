#pragma once

class ConfigurationCommand : public Command
{
public:

    ConfigurationCommand() : Command("config", "Manage your configurations", { "c", "configuration" })
    {}

    bool execute(std::vector<std::string> cmd) override
    {
        if (cmd.size() == 0)
            return false;

        static std::string SavedConfig = "default";

        //.config save
        if (strcmp(cmd[0].c_str(), "save") == 0 || strcmp(cmd[0].c_str(), "s") == 0)
        {
            if (cmd.size() == 2)
            {
                SavedConfig = cmd[1];
                mConfigMgr.SaveConfig(cmd[1]);
            }
            else
            {
                mConfigMgr.SaveConfig(SavedConfig);
            }
        }

        //.config load
        if (strcmp(cmd[0].c_str(), "load") == 0 || strcmp(cmd[0].c_str(), "l") == 0)
        {
            if (cmd.size() == 2)
            {
                SavedConfig = cmd[1];
                mConfigMgr.LoadConfig(cmd[1]);
            }
        }

        return false;
    }
};