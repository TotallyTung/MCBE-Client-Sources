//
// Created by vastrakai on 10/17/2023.
//

#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <imgui_internal.h>
#include <Prax/Utils/Render/Render.h>
#include <trigonometric.hpp>
#include <Prax/SDK/Game/Network/Other/MovePlayerPacket.h>
#include <Prax/Utils/Math.h>
#include "InfiniteAura.h"
#include "Antibot.h"

bool InfiniteAura::HasTarget = false;
Vector3 InfiniteAura::ServersidePos = {0.f, 0.f, 0.f};
Vector3 InfiniteAura::LastPos = {0.f, 0.f, 0.f};
int InfiniteAura::EaseTicks = 10; // The amount of ticks it takes to ease the position to the target
__int64 InfiniteAura::nextHitTime = 0;
Vector3 InfiniteAura::TargetPos = {0.f, 0.f, 0.f};
__int64 InfiniteAura::LastLagbackTime = 0;

bool InfiniteAura::CanRaycastTo(Actor* actor){
    auto player = Minecraft::ClientInstance->ThePlayer();

    AABB aabb = player->GetAABB();
    AABB serverAabb = player->GetAABBForPos(ServersidePos);
    player->SetAABB(serverAabb);
    bool result = player->canSee(actor);
    player->SetAABB(aabb);
    return result;
}

Vector3 LerpVec3(Vector3 a, Vector3 b, float t) {
    return {a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t};
}
// normalize a vector
Vector3 normalizeVec3(Vector3 vec) {
    float length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    return {vec.x / length, vec.y / length, vec.z / length};
}

void InfiniteAura::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    auto entityList = Antibot::GetActors();

    float blocksPerTp = 2.f;

    // sort the entitylist by distance to ServerSidePos
    std::sort(entityList.begin(), entityList.end(), [&](Actor *a, Actor *b) {
        return player->getPos()->Distance(*a->getPos()) < player->getPos()->Distance(*b->getPos());
    });

    // use Misc::IsOverVoid(vector3 pos) for each player and remove them if they are over void
    for (auto actor : entityList) {
        if (Misc::IsOverVoid(*actor->getPos())) {
            entityList.erase(std::remove(entityList.begin(), entityList.end(), actor), entityList.end());
        }
    }

    if (HurtTime.Enabled) {
        // Erase actors that have a hurt time of more than 0
        for (auto actor : entityList) {
            if (*actor->hurtTime - HurtTimeCompensation.Value > 0) {
                entityList.erase(std::remove(entityList.begin(), entityList.end(), actor), entityList.end());
            }
        }
    }

    if (RaycastCheck.Enabled) {
        for (auto actor: entityList) {
            if (!CanRaycastTo(actor)) {
                entityList.erase(std::remove(entityList.begin(), entityList.end(), actor), entityList.end());
            }
        }
    }

    // exclude the player from the entitylist
    entityList.erase(std::remove(entityList.begin(), entityList.end(), player), entityList.end());

    // if the entitylist is empty, return
    if (entityList.empty()) {
        // ease the position back to the player

        LastPos = ServersidePos;
        ServersidePos = LerpVec3(ServersidePos, *player->getPos(), ImGui::GetIO().DeltaTime * EasingTicks.Value);

        if (ServersidePos.Distance(*player->getPos()) < 3.f) {
            LastPos = ServersidePos;
            ServersidePos = *player->getPos();
        } else {
            return;
        }

        HasTarget = false;
        return;
    }

    // if the entitylist is not empty, set the target to the first entity in the list
    HasTarget = true;
    auto target = entityList[0];


    // if the LastLagbackTime is less than 100ms ago, return
    if (LastLagbackTime > 0 && LastLagbackTime + 100 > Misc::GetCurrentMs() && SilentAcceptLagbacks.Enabled) {
        return;
    }

    // if the target is null, return
    if (!target) {
        HasTarget = false;
        return;
    }

    // if the target is not null, start easing the position to the target using the EaseTicks variable
    HasTarget = true;

    TargetPos = *target->getPos() - Vector3(0.f, 1.62f, 0.f);
    float distance = ServersidePos.Distance(*target->getPos());


    // if the serversidePos is within 3 blocks of the target, offset the position based on the target's rotation
    // so that the serversidePos is directly behind the player
    if (target->getPos()->Distance(ServersidePos) < AttackDistance.Value) {
        float yawDeg = target->GetActorRotationComponent()->yaw;
        // clamp the yaw to -180 to 180
        while (yawDeg > 180.f) yawDeg -= 360.f;
        float pitchDeg = target->GetActorRotationComponent()->pitch;
        // clamp the pitch to -80 to 80
        while (pitchDeg > 80.f) pitchDeg -= 160.f;
        float distance = 3.f;
        float yaw = yawDeg * (IM_PI / 180.0f) + 90.f;
        float pitch = pitchDeg * (IM_PI / 180.0f);
        Vector3 newPacketPos = *target->getPos();
        newPacketPos.x -= cos(yaw) * cos(pitch) * distance;
        newPacketPos.y -= sin(pitch) * distance - 1.62f;
        newPacketPos.z -= sin(yaw) * cos(pitch) * distance;
        // if the packet pos is inside a block, find a pos within the AttackDistance that is air
        if (!Misc::IsAirBlock(newPacketPos)) {
            bool newPosFound = false;
            // check all the blocks within the AttackDistance and find the first air block
            for (int x = -AttackDistance.Value; x < AttackDistance.Value; x++) {
                for (int y = -AttackDistance.Value; y < AttackDistance.Value; y++) {
                    for (int z = -AttackDistance.Value; z < AttackDistance.Value; z++) {
                        Vector3 pos = *target->getPos();
                        pos.x += x;
                        pos.y += y;
                        pos.z += z;
                        if (Misc::IsAirBlock(pos) && Misc::IsAirBlock(pos + Vector3(0.f, 1.f, 0.f))) {
                            newPacketPos = pos;
                            newPosFound = true;
                            break;
                        }
                    }
                    if (newPosFound) break;
                }
                if (newPosFound) break;
            }

            // if no air block was found, return
            if (!newPosFound) {
                return;
            }
        }

        LastPos = ServersidePos;
        ServersidePos = newPacketPos;

        __int64 currentTime = Misc::GetCurrentMs();
        auto aps = (__int64)(1000 / APS.Value);

        if (currentTime > nextHitTime || abs(currentTime - nextHitTime) < 15) {
            nextHitTime = currentTime + aps;
        } else {
            return;
        }

        player->swing();
        player->gamemode->attack(target);
    } else {
        LastPos = ServersidePos;
        //ServersidePos = LerpVec3(ServersidePos, *target->getPos(), ImGui::GetIO().DeltaTime * EasingTicks.Value);
        // Teleport forward x blocks to the target pos
        Vector3 direction = normalizeVec3(*target->getPos() - ServersidePos);
        ServersidePos.x += direction.x * blocksPerTp;
        ServersidePos.y += direction.y * blocksPerTp;
        ServersidePos.z += direction.z * blocksPerTp;



    }
}

void InfiniteAura::OnEnable() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    ServersidePos = *player->getPos();
    LastPos = ServersidePos;
}

void InfiniteAura::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    if (packet->getId() == (int)PacketID::MovePlayer) {
        auto mpp = std::static_pointer_cast<MovePlayerPacket>(packet);
        if (mpp->position == Vector3(0.f, 0.f, 0.f)) return;
        // Disable the module if the pos is more than 500 blocks away
        if (mpp->position.Distance(ServersidePos) > 500.f) {
            this->Toggle();
            return;
        }
        if (!SilentAcceptLagbacks.Enabled) return;

        LastLagbackTime = Misc::GetCurrentMs();
        LastPos = ServersidePos;
        ServersidePos = mpp->position;
        *cancel = true;
    }
}

void InfiniteAura::OnPacket(class LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (packet->getId() == (int)PacketID::PlayerAuthInput) {
        auto paip = (PlayerAuthInputPacket *) packet;
        if (paip->position == Vector3(0.f, 0.f, 0.f)) return;
        if (HasTarget) {
            paip->position = ServersidePos;
            paip->rotation = Misc::GetRotations(&ServersidePos, &TargetPos);
            paip->headYaw = paip->rotation.y;
        } else {
            LastPos = ServersidePos;
            ServersidePos = paip->position;
        }
    }
}

void InfiniteAura::OnRender() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    Vector3 lerpedPos = LerpVec3(LastPos, ServersidePos, Minecraft::ClientInstance->GetDeltaTime());
    AABB aabb1 = player->GetAABBForPos(lerpedPos);
    AABB aabb2 = player->GetAABBForPos(TargetPos);
    Math::DrawBox(aabb1.lower, aabb1.upper, 1.0f, ImColor(255, 0, 0, 255));
    Math::DrawBox(aabb2.lower, aabb2.upper, 1.0f, ImColor(255, 255, 0, 255));
}