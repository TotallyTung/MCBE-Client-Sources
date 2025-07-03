//
// Created by alteik on 21/09/2024.
//

#include "Extinguisher.hpp"
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/Actor/GameMode.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/World/BlockLegacy.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>

bool Extinguisher::isValidBlock(glm::ivec3 blockPos)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    Block* block = ClientInstance::get()->getBlockSource()->getBlock(blockPos);
    if (!block) return false;

    if (block->getmLegacy()->isAir()) return false;

    int blockId = block->getmLegacy()->getBlockId();
    if(blockId != 51) return false;

    AABB blockAABB = AABB(blockPos, glm::vec3(1, 1, 1));
    glm::vec3 closestPos = blockAABB.getClosestPoint(*player->getPos());
    if (mRange.mValue < glm::distance(closestPos, *player->getPos())) return false;

    int exposedFace = BlockUtils::getExposedFace(blockPos);
    if (exposedFace == -1) return false;

    return true;
}

void Extinguisher::onEnable() {
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Extinguisher::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &Extinguisher::onPacketOutEvent>(this);
}

void Extinguisher::onDisable() {
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Extinguisher::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &Extinguisher::onPacketOutEvent>(this);
}

void Extinguisher::onBaseTickEvent(BaseTickEvent &event) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if(!player) return;
    if(player->isDestroying()) return;

    std::vector<BlockInfo> blockList = BlockUtils::getBlockList(*player->getPos(), mRange.mValue);
    std::vector<BlockInfo> exposedBlockList;

    for (const auto& block : blockList) {
        if (isValidBlock(block.mPosition)) {
            exposedBlockList.push_back(block);
            break;
        }
    }

    if(exposedBlockList.empty()) return;

    mCurrentPosition = exposedBlockList[0].mPosition;
    BlockUtils::startDestroyBlock(mCurrentPosition, -6);
    mShouldRotate = mRotate.mValue;
}

void Extinguisher::onPacketOutEvent(PacketOutEvent &event) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if(!player) return;

    if (event.mPacket->getId() == PacketID::PlayerAuthInput)
    {
        auto paip = event.getPacket<PlayerAuthInputPacket>();
        if (mShouldRotate)
        {
            const glm::vec3 blockPos = mCurrentPosition;
            auto blockAABB = AABB(blockPos, glm::vec3(1, 1, 1));
            glm::vec2 rotations = MathUtils::getRots(*player->getPos(), blockAABB);
            paip->mRot = rotations;
            paip->mYHeadRot = rotations.y;
            mShouldRotate = false;
        }
    }
}