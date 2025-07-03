//
// Created by vastrakai on 11/9/2024.
//

#include "TeleportCommand.hpp"

#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>

void TeleportCommand::execute(const std::vector<std::string>& args)
{
    auto player = ClientInstance::get()->getLocalPlayer();

    if (args.size() < 2)
    {
        ChatUtils::displayClientMessage("§c" + getUsage());
        return;
    }

    if (args.size() == 2)
    {
        auto targetList = ActorUtils::getActorList(true, true);
        Actor* target = nullptr;
        std::string targetName = StringUtils::toLower(args[1]);
        for (auto& actor : targetList)
        {
            if (StringUtils::toLower(actor->getRawName()) == targetName)
            {
                target = actor;
                break;
            }
        }

        if (target == nullptr)
        {
            ChatUtils::displayClientMessage("§cCould not find player: " + args[1] + "!");
            return;
        }

        player->setPosition(*target->getPos());
        ChatUtils::displayClientMessage("§aTeleported to " + target->getRawName() + "!");
        return;
    }
    else if (args.size() != 4)
    {
        ChatUtils::displayClientMessage("§c" + getUsage());
        return;
    }

    try
    {
        auto comp = player->getActorRotationComponent();
        glm::vec2 rotation = { comp->mPitch, comp->mYaw };

        glm::vec3 pos = *player->getPos();
        std::string xStr = args[1];
        std::string yStr = args[2];
        std::string zStr = args[3];

        // if the string only contains a symbol, append 0 to it
        if (xStr.size() == 1) xStr += "0";
        if (yStr.size() == 1) yStr += "0";
        if (zStr.size() == 1) zStr += "0";

        // if number starts with ~, add to current position
        // if number starts with ^, make the pos relative to the player's rotation
        if (xStr[0] == '~')
        {
            pos.x += std::stof(xStr.substr(1));
        }
        else if (xStr[0] == '^')
        {
            pos.x += std::stof(xStr.substr(1)) * cos(glm::radians(rotation.y + 90));
            pos.z += std::stof(xStr.substr(1)) * sin(glm::radians(rotation.y + 90));
        }
        else
        {
            pos.x = std::stof(xStr);
        }

        if (yStr[0] == '~')
        {
            pos.y += std::stof(yStr.substr(1));
        }
        else if (yStr[0] == '^')
        {
            pos.y += std::stof(yStr.substr(1)) * sin(glm::radians(rotation.x));
        }
        else
        {
            pos.y = std::stof(yStr);
        }

        if (zStr[0] == '~')
        {
            pos.z += std::stof(zStr.substr(1));
        }
        else if (zStr[0] == '^')
        {
            pos.x += std::stof(zStr.substr(1)) * cos(glm::radians(rotation.y + 90));
            pos.z += std::stof(zStr.substr(1)) * sin(glm::radians(rotation.y + 90));
        }
        else
        {
            pos.z = std::stof(zStr);
        }

        player->setPosition(pos);
        ChatUtils::displayClientMessage("§aTeleported to " + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + "!");
    } catch (const std::exception& e)
    {
        ChatUtils::displayClientMessage("§cInvalid coordinates entered!");
        return;
    }
}

std::vector<std::string> TeleportCommand::getAliases() const
{
    return { "tp" };
}

std::string TeleportCommand::getDescription() const
{
    return "Teleport to a player or coordinates.";
}

std::string TeleportCommand::getUsage() const
{
    return "Usage: teleport <player> OR teleport <x> <y> <z> (Also supports ~ and ^ operators)";
}
