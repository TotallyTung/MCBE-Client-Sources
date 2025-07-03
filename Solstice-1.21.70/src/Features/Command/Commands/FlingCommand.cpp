//
// Created by vastrakai on 8/30/2024.
//

#include "FlingCommand.hpp"

#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>

void FlingCommand::execute(const std::vector<std::string>& args)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    glm::vec2 motion = MathUtils::getMotion(player->getActorRotationComponent()->mYaw, 4.f, true);
    player->getStateVectorComponent()->mVelocity = {motion.x, 1.f, motion.y};
}

std::vector<std::string> FlingCommand::getAliases() const
{
    return {};
}

std::string FlingCommand::getDescription() const
{
    return "Flings you into the air";
}

std::string FlingCommand::getUsage() const
{
    return "Usage: .fling";
}
