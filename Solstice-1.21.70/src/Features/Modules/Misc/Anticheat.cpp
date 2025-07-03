//
// Created by ssi on 10/23/2024.
//

#include "Anticheat.hpp"
#include <Features/FeatureManager.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Network/Packets/Packet.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/MinecraftSim.hpp>
#include <unordered_set>

void Anticheat::onEnable() {
    gFeatureManager->mDispatcher->listen<PacketInEvent, &Anticheat::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Anticheat::onBaseTickEvent>(this);
}

void Anticheat::onDisable() {
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &Anticheat::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Anticheat::onBaseTickEvent>(this);
}

float calculateBps(Actor* actor, const glm::vec3& lastPosition) {
    glm::vec3 currentPos = *actor->getPos();
    return glm::distance(glm::vec2(currentPos.x, currentPos.z), glm::vec2(lastPosition.x, lastPosition.z))
           * (ClientInstance::get()->getMinecraftSim()->getSimTimer() * ClientInstance::get()->getMinecraftSim()->getSimSpeed());
}

void Anticheat::onBaseTickEvent(BaseTickEvent& event) {
    auto actors = ActorUtils::getActorList(true, true);
    auto localPlayer = ClientInstance::get()->getLocalPlayer();
    int64_t currentTime = NOW;

    std::unordered_set<Actor*> currentActors(actors.begin(), actors.end());
    for (auto it = playerMap.begin(); it != playerMap.end();) {
        if (currentActors.find(it->first) == currentActors.end()) {
            it = playerMap.erase(it);
        } else {
            ++it;
        }
    }

    for (auto actor : actors) {
        if (!actor->isPlayer() || actor == localPlayer) {
            continue;
        }

        if (playerMap.find(actor) == playerMap.end()) {
            playerMap[actor] = PlayerInfo();
            playerMap[actor].playerName = actor->getRawName();
            playerMap[actor].lastPosition = *actor->getPos();
        }

        auto& playerInfo = playerMap[actor];
        glm::vec3 currentPosition = *actor->getPos();
        glm::vec3 lastPosition = playerInfo.lastPosition;

        auto rotationComponent = actor->getActorRotationComponent();
        float yaw = rotationComponent->mYaw;
        float pitch = rotationComponent->mPitch;

        if (mRotationCheck.mValue && (yaw <= -181.0f || yaw >= 181.0f || pitch >= 90.0f || pitch <= -90.0f)) {
            int64_t timeSinceLastFlag = currentTime - playerInfo.lastFlagTime;

            if (timeSinceLastFlag >= 3000) {
                playerInfo.flaggedChecks.insert(Checks::ROTATION);
                playerInfo.flagCounts[Checks::ROTATION]++;

                if (playerInfo.flagCounts[Checks::ROTATION] >= 5) {
                    ChatUtils::displayClientMessage(fmt::format("§c{} §7flagged §c{} (Yaw: {:.2f}, Pitch: {:.2f})", playerInfo.playerName, "ROTATION", yaw, pitch));
                    playerInfo.flagCounts[Checks::ROTATION] = 0;
                    playerInfo.flaggedChecks.clear();
                }
                playerInfo.lastFlagTime = currentTime;
            }
        }

        bool isImmobile = actor->getStatusFlag(ActorFlags::Noai);
        if (mImmobileCheck.mValue && isImmobile && glm::distance(currentPosition, lastPosition) > 0.1f) {
            int64_t timeSinceLastFlag = currentTime - playerInfo.lastFlagTime;

            if (timeSinceLastFlag >= 2000) {
                playerInfo.flaggedChecks.insert(Checks::IMMOBILE);
                playerInfo.flagCounts[Checks::IMMOBILE]++;

                if (playerInfo.flagCounts[Checks::IMMOBILE] >= 1) {
                    ChatUtils::displayClientMessage(fmt::format("§c{} §7flagged §c{}", playerInfo.playerName, "IMMOBILE"));
                    playerInfo.flagCounts[Checks::IMMOBILE] = 0;
                    playerInfo.flaggedChecks.clear();
                }
                playerInfo.lastFlagTime = currentTime;
            }
        }

        float distanceMoved = glm::distance(currentPosition, lastPosition);
        if (distanceMoved <= 0.5f || distanceMoved > 2.0f) {
            playerInfo.lastPosition = currentPosition;
            continue;
        }

        float bps = calculateBps(actor, playerInfo.lastPosition);
        playerInfo.bpsHistory[currentTime] = bps;
        playerInfo.lastPosition = currentPosition;

        for (auto it = playerInfo.bpsHistory.begin(); it != playerInfo.bpsHistory.end();) {
            if (currentTime - it->first > 1000) {
                it = playerInfo.bpsHistory.erase(it);
            } else {
                ++it;
            }
        }

        float totalBps = 0.f;
        int bpsCount = 0;
        for (const auto& entry : playerInfo.bpsHistory) {
            totalBps += entry.second;
            bpsCount++;
        }

        auto mobHurtTimeComp = actor->getMobHurtTimeComponent();
        if (!mobHurtTimeComp) continue;

        int hurtTime = mobHurtTimeComp->mHurtTime;
        bool isOnGround = actor->isOnGround();
        float speedLimit = isOnGround ? 7.0f : 9.0f;

        if (mSpeedBCheck.mValue && mSpeedCheck.mValue && isOnGround && hurtTime == 0 && totalBps / bpsCount > speedLimit) {
            int64_t timeSinceLastFlag = currentTime - playerInfo.lastFlagTime;
            if (timeSinceLastFlag >= 1000) {
                playerInfo.flaggedChecks.insert(Checks::SPEED_B);
                playerInfo.flagCounts[Checks::SPEED_B]++;

                if (playerInfo.flagCounts[Checks::SPEED_B] >= 8) {
                    ChatUtils::displayClientMessage(fmt::format("§c{} §7flagged §c{} §8(BPS: {:.2f})", playerInfo.playerName, "SPEED-B (On-Ground)", bps));
                    playerInfo.flagCounts[Checks::SPEED_B] = 0;
                    playerInfo.flaggedChecks.clear();
                }
                playerInfo.lastFlagTime = currentTime;
            }
        }

        if (mSpeedACheck.mValue && mSpeedCheck.mValue && !isOnGround && hurtTime == 0 && totalBps / bpsCount > speedLimit) {
            int64_t timeSinceLastFlag = currentTime - playerInfo.lastFlagTime;
            if (timeSinceLastFlag >= 1000) {
                playerInfo.flaggedChecks.insert(Checks::SPEED_A);
                playerInfo.flagCounts[Checks::SPEED_A]++;

                if (playerInfo.flagCounts[Checks::SPEED_A] >= 8) {
                    ChatUtils::displayClientMessage(fmt::format("§c{} §7flagged §c{} §8(BPS: {:.2f})", playerInfo.playerName, "SPEED-A (Off-Ground)", bps));
                    playerInfo.flagCounts[Checks::SPEED_A] = 0;
                    playerInfo.flaggedChecks.clear();
                }
                playerInfo.lastFlagTime = currentTime;
            }
        }

        if (playerInfo.lastFlagTime > 0 && (currentTime - playerInfo.lastFlagTime > 15000)) {
            for (auto& flagCount : playerInfo.flagCounts) {
                flagCount.second = 0;
            }
            playerInfo.flaggedChecks.clear();
            playerInfo.lastFlagTime = 0;
        }
    }
}

void Anticheat::onPacketInEvent(PacketInEvent& event) {
    if (event.mPacket->getId() == PacketID::ChangeDimension) {
        for (auto& [actor, playerInfo] : playerMap) {
            for (auto& flagCount : playerInfo.flagCounts) {
                flagCount.second = 0;
            }
            playerInfo.flaggedChecks.clear();
            playerInfo.lastFlagTime = 0;
        }
    }
}