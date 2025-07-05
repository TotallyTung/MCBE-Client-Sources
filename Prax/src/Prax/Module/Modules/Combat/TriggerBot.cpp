//
// Created by vastrakai on 1/27/2024.
//

#include "TriggerBot.h"
#include <Prax/SDK/Game/Minecraft.h>
#include <Prax/Module/Modules/World/Scaffold.h>
#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/Render.h>
#include <Prax/Utils/Misc.h>
#include "Antibot.h"

#define NOW Misc::GetCurrentMs()

Actor* TriggerBot::getActorFromEntityId(EntityId entityId)
{
    auto player = Minecraft::ClientInstance->ThePlayer();



    if (UseAntibot.Enabled)
    {
        for (auto actor : Antibot::GetActors())
        {
            if (actor->entityCtx.entityId == entityId)
            {
                return actor;
            }
        }
    } else
    {
        for (auto actor : player->level->getRuntimeActorList())
        {
            if (actor->entityCtx.entityId == entityId)
            {
                return actor;
            }
        }
    }

    std::string serverIp = Minecraft::ClientInstance->packetSender->networkSystem->getConnectionInfo()->serverIp;
    if (serverIp.find(xorstr_("hive")) != std::string::npos)
    {
        for (auto actor : player->level->getRuntimeActorList())
        {
            if (actor->entityCtx.entityId == entityId)
            {
                if (actor->GetEntityId() != "minecraft:pig") continue;
                return actor;

            }
        }
    }

    return nullptr;
}

void TriggerBot::OnTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (MinAps.Value > MaxAps.Value)
    {
        player->addChatMessage("&cMinAps cannot be greater than MaxAps!");
        MinAps.Value = MaxAps.Value;
    }

    HitResult* hitResult = player->level->getHitResult();

    if (HoldLeftClick.Enabled && !ImGui::IsMouseDown(0)) return;

    int rngAps = Math::RandomInt(MinAps.Value, MaxAps.Value);

    if (hitResult->type == RaycastTypes::ENTITY)
    {
        Actor* hitActor = getActorFromEntityId(hitResult->entity.id);
        if (!hitActor) return;

        if (hitActor->entityCtx.entityId == player->entityCtx.entityId) return;
        static uint64_t lastAttack = 0;
        if (NOW - lastAttack < 1000 / rngAps) return;

        player->swing();
        player->gamemode->attack(hitActor);

    }

}

void TriggerBot::OnEnable()
{

}

void TriggerBot::OnDisable()
{

}
