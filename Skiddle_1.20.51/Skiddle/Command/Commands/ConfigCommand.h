#pragma once

class ConfigCommand : public Command
{
public:

    ConfigCommand() : Command("config", "Manage your configurations", { "c" })
    {}

    std::string nowConfig = "default";

    bool execute(std::vector<std::string> cmd) override
    {
        if (cmd.size() == 0)
            return false;

        //.config save
        if (strcmp(cmd[0].c_str(), "save") == 0 || strcmp(cmd[0].c_str(), "s") == 0)
        {
            if (cmd.size() == 2)
            {
                SaveCfg(cmd[1]);
            }
            else
            {
                SaveCfg(nowConfig);
            }
        }

        //.config new
        if (strcmp(cmd[0].c_str(), "new") == 0 || strcmp(cmd[0].c_str(), "n") == 0)
        {
            if (cmd.size() == 2)
            {
                SaveConfigNow(nowConfig);
                CreateCfg(cmd[1]);
                LoadConfig2(cmd[1]);
                nowConfig = cmd[1];
            }
        }

        //.config load
        if (strcmp(cmd[0].c_str(), "load") == 0 || strcmp(cmd[0].c_str(), "l") == 0)
        {
            if (cmd.size() == 2)
            {
                // SaveConfigNow(nowConfig);
                LoadConfig(cmd[1]);
                nowConfig = cmd[1];
            }
            else
            {
                LoadConfig("default");
            }
        }

        return false;
    }

    void SaveCfg(std::string cfg)
    {
        SaveConfig(cfg);
        ChatUtil::sendMessage(combine("Saved configuration ", BOLD, cfg.c_str()));
    }

    void CreateCfg(std::string cfg)
    {
        SaveConfig(cfg);
        ChatUtil::sendMessage(combine("Created configuration ", BOLD, cfg.c_str()));
    }
};
