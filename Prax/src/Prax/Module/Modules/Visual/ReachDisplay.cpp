//
// Created by vastrakai on 11/12/2023.
//

#include "ReachDisplay.h"

void ReachDisplay::OnEnable()
{

}

void ReachDisplay::OnDisable()
{

}

float getDistanceFromAABB(AABB aabb, Vector3 pos) {
    float x = std::max(aabb.lower.x - pos.x, pos.x - aabb.upper.x);
    float y = std::max(aabb.lower.y - pos.y, pos.y - aabb.upper.y);
    float z = std::max(aabb.lower.z - pos.z, pos.z - aabb.upper.z);
    return sqrt(x * x + y * y + z * z);
}

Actor* getActorFromRuntimeId(uint64_t runtimeId)
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    for (auto& actor : player->level->getRuntimeActorList()) {
        if (actor->GetRuntimeID() == runtimeId) return actor;
    }

    return nullptr;
}

float lastDistance = 0;

void ReachDisplay::OnAttack(Actor* actor)
{
    Logger::Write("ReachDisplay::OnAttack", "Called", Logger::Debug);
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (!actor || !player) return;

    lastDistance = getDistanceFromAABB(actor->GetAABB(), *player->getPos());

    player->addChatMessage("Distance: " + std::to_string(lastDistance), 0);
}

void ReachDisplay::OnTick()
{

}
