//
// Created by alteik on 15/10/2024.
//

#include "ClickTp.hpp"
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>
#include <SDK/Minecraft/World/Level.hpp>

void ClickTp::onEnable()
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    HitResult* hitResult = player->getLevel()->getHitResult();
    glm::vec3 newPos = {0, 0, 0};

    if(hitResult->mType == HitType::BLOCK)
    {
        glm::vec3 blockPos = hitResult->mBlockPos;
        newPos = {blockPos.x, blockPos.y + 1.01f + PLAYER_HEIGHT, blockPos.z};

        player->setPosition(newPos);
    }

    setEnabled(false);
}