//
// Created by Flash on 7/30/2023.
//

#include "Command.h"

Command::Command(const std::string& trigger, const std::string& description, const std::string& usage,
    const std::vector<std::string>& aliases)
{
    Trigger = trigger;
    Description = description;
    Aliases = aliases;


    if (usage.empty())
        CreateUsage();
    else
        Usage = usage;
}

void Command::AddArgument(const std::string& name, const std::string& type)
{
    Arguments.insert(std::pair<std::string, std::string>(name, type));
    CreateUsage();
}

void Command::CreateUsage()
{
    Usage = std::string("." + Trigger);

    // Reverse the map so that the arguments are in the correct order
    std::vector<std::pair<std::string, std::string>> reversed;
    for (auto& arg : Arguments) {
        reversed.insert(reversed.begin(), arg);
    }

    for (auto& arg : reversed) {
        Usage += " <" + arg.first + ">";
    }
}

void Command::ShowUsage(const std::function<void()>& callback)
{
    if (callback != nullptr)
        callback();
}

void Command::AddCommandMessage(std::string message, std::string sender) const
{
    if (sender == "Prax")
    {
        sender = "§r§l§6" + Trigger + "§f§l";
        // Make the first letter of sender a capital
        sender[9] = toupper(sender[9]);
    }
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (!player) return;

    Logger::Write(sender, message);
    player->addChatMessage(sender, message, TextUtils::White, true);

}
