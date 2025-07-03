//
// Created by ssi on 10/24/2024.
//

#include "ChestAura.hpp"
#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Actor/GameMode.hpp>
#include <SDK/Minecraft/Network/PacketID.hpp>
#include <SDK/Minecraft/World/BlockLegacy.hpp>

void ChestAura::onEnable() {
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &ChestAura::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &ChestAura::onPacketInEvent>(this);
}

void ChestAura::onDisable() {
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &ChestAura::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &ChestAura::onBaseTickEvent>(this);

    mOpenedChestPositions.clear();
    mIsChestOpened = false;
    mTimeOfLastChestOpen = 0;
}

void ChestAura::onBaseTickEvent(BaseTickEvent& event) {
    if (NOW - mTimeOfLastChestOpen < mDelay.mValue) return;

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player || player->getStatusFlag(ActorFlags::Noai) || ClientInstance::get()->getScreenName() != "hud_screen") return;

    const glm::vec3 playerPos = *player->getPos();
    std::vector<glm::vec3> chests;

    for (int x = static_cast<int>(playerPos.x) - mRange.mValue; x <= static_cast<int>(playerPos.x) + mRange.mValue; ++x) {
        for (int y = static_cast<int>(playerPos.y) - mRange.mValue; y <= static_cast<int>(playerPos.y) + mRange.mValue; ++y) {
            for (int z = static_cast<int>(playerPos.z) - mRange.mValue; z <= static_cast<int>(playerPos.z) + mRange.mValue; ++z) {
                glm::vec3 blockPos = glm::vec3(x, y, z);
                Block* block = ClientInstance::get()->getBlockSource()->getBlock(x, y, z);

                if (block->mLegacy->getBlockId() == 0 || !block->mLegacy->mName.contains("chest")) continue;
                if (std::find(mOpenedChestPositions.begin(), mOpenedChestPositions.end(), blockPos) != mOpenedChestPositions.end()) continue;

                chests.push_back(blockPos);
            }
        }
    }

    if (chests.empty() || mIsChestOpened) return;

    glm::vec3 closestChest = chests[0];
    float closestDistance = glm::distance(closestChest, playerPos);

    for (const auto& chestPos : chests) {
        float distance = glm::distance(chestPos, playerPos);
        if (distance < closestDistance) {
            closestDistance = distance;
            closestChest = chestPos;
        }
    }

    int nearestFace = -1;
    float minDistance = std::numeric_limits<float>::max();

    for (int i = 0; i < BlockUtils::blockFaceOffsets.size(); ++i) {
        glm::vec3 facePos = closestChest - BlockUtils::blockFaceOffsets[i];
        float distance = glm::distance(facePos, playerPos);
        if (distance < minDistance) {
            minDistance = distance;
            nearestFace = i;
        }
    }

    if (nearestFace != -1) {
        player->getGameMode()->buildBlock(closestChest, nearestFace, false);
        mOpenedChestPositions.push_back(closestChest);
        mTimeOfLastChestOpen = NOW;
    }
}

void ChestAura::onPacketInEvent(PacketInEvent& event) {
    if (event.mPacket->getId() == PacketID::ChangeDimension) {
        mOpenedChestPositions.clear();
        mIsChestOpened = false;
    } else if (event.mPacket->getId() == PacketID::ContainerOpen) {
        mIsChestOpened = true;
    } else if (event.mPacket->getId() == PacketID::ContainerClose) {
        mIsChestOpened = false;
    }
}