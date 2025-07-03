//
// Created by alteik on 03/09/2024.
//

#include "VclipCommand.hpp"
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>

void VclipCommand::execute(const std::vector<std::string>& args)
{
    if (args.size() == 2)
    {
        const std::string& amountOfBlocks = args[1];
        if(!isNumber(amountOfBlocks))
        {
            ChatUtils::displayClientMessage("§cInvalid input. Please enter a number!");
        }

        auto player = ClientInstance::get()->getLocalPlayer();
        auto pos = *player->getPos();
        auto amount = std::stof(amountOfBlocks);
        player->setPosition({pos.x, pos.y+ amount, pos.z});

        bool negative = amount < 0;
        std::string direction = negative ? "down" : "up";
        int absValue = abs(amount);
        ChatUtils::displayClientMessage("§aTeleported §7" + direction + " §6" + std::to_string(absValue) + " §ablocks.");
    }
}

std::vector<std::string> VclipCommand::getAliases() const
{
    return {"v"};
}

std::string VclipCommand::getDescription() const
{
    return "Teleport yourself up or down by a certain amount of blocks.";
}

std::string VclipCommand::getUsage() const
{
    return "Usage .vclip <amount of blocks> (ex: 2, -2)";
}

bool VclipCommand::isNumber(std::string str) {
    try {
        return std::stof(str);
    } catch (std::invalid_argument &e) {
        return false;
    }
}