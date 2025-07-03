//
// Created by vastrakai on 6/25/2024.
//

#include "ChatUtils.hpp"

#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Rendering/GuiData.hpp>

void ChatUtils::displayClientMessage(const std::string& msg)
{
    if (!msg.contains("\n"))
    {// FUCK YOU DONT CHANGE THE PREIX COLOR U NIGGER
        ClientInstance::get()->getGuiData()->displayClientMessageQueued("§asolstice§7 » §r" + msg);
        return;
    }

    // Newline handling
    std::string formattedMsg = "§asolstice§7 » §r";
    for (const auto& c : msg)
    {
        if (c == '\n')
        {
            formattedMsg += "\n§asolstice§7 » §r";
        }
        else
        {
            formattedMsg += c;
        }
    }
    ClientInstance::get()->getGuiData()->displayClientMessageQueued(formattedMsg);
}

void ChatUtils::displayClientMessageSub(const std::string& subcaption, const std::string& msg)
{
    ClientInstance::get()->getGuiData()->displayClientMessageQueued("§asolstice§7 » §7[" + subcaption + "§7] §r" + msg);
}

void ChatUtils::displayClientMessageRaw(const std::string& msg)
{
    ClientInstance::get()->getGuiData()->displayClientMessageQueued(msg);
}



