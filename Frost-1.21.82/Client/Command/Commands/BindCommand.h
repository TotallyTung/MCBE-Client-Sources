#pragma once

class BindCommand : public Command
{
public:

    BindCommand() : Command("bind", "Bind a module to a key.", { "b" })
    {}

    bool execute(std::vector<std::string> cmd) override // bro has no error handling with chat messages imagineeee
    {
        if (cmd.size() < 2)
            return false;

        const auto& mod = getModuleByName(cmd[0]);
        if (mod == nullptr) {
            ChatUtil::sendMessage(Util::combine("Could not find module with the name: ", TextFormat::getColorCode(TextFormat::Color::GRAY), cmd[0].c_str()));
            return false;
        }

        if (strcmp(cmd[1].c_str(), "none") == 0)
        {
            mod->setKeybind(0);
            ChatUtil::sendMessage(Util::combine("Successfully unbound", TextFormat::getColorCode(TextFormat::Color::GRAY), mod->getName()));
            return true;
        }

        if (cmd[1].length() == 1)
        {
            if (cmd[1][0] > 96 && cmd[1][0] < 122)
                std::transform(cmd[1].begin(), cmd[1].end(), cmd[1].begin(), [](unsigned char c) { return::toupper(c); });
        }

        std::string cmdLower = cmd[1];
        std::transform(cmdLower.begin(), cmdLower.end(), cmdLower.begin(), [](unsigned char c) { return std::tolower(c); });

        for (int i = 0; i < 193; i++)
        {
            std::string keyLower = KeyName[i];
            std::transform(keyLower.begin(), keyLower.end(), keyLower.begin(), [](unsigned char c) { return std::tolower(c); });

            if (cmdLower == keyLower)
            {
                mod->setKeybind(i);
                ChatUtil::sendMessage(Util::combine("The Keybind of ", TextFormat::getColorCode(TextFormat::Color::GRAY), mod->getName(), TextFormat::getStyleCode(TextFormat::Style::RESET), " is now ", TextFormat::getColorCode(TextFormat::Color::GRAY), "'", KeyName[i], "'"));
                return true;
            }
        }

        ChatUtil::sendMessage("Invalid key!");
        return false;
    }
};
