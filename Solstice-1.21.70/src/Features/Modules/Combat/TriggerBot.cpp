//
// Created by alteik on 04/09/2024.
//

#include "TriggerBot.hpp"
#include <Features/Events/BaseTickEvent.hpp>
#include <Utils/GameUtils/ActorUtils.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/GameMode.hpp>
#include <SDK/Minecraft/World/Level.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>
#include <Features/Modules/Misc/AntiBot.hpp>

void TriggerBot::onEnable() {
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &TriggerBot::onBaseTickEvent>(this);
}

void TriggerBot::onDisable() {
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &TriggerBot::onBaseTickEvent>(this);
}

Actor* TriggerBot::getActorFromEntityId(EntityId entityId)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    auto antiBot = gFeatureManager->mModuleManager->getModule<AntiBot>();
    if (antiBot == nullptr)
    {
        ChatUtils::displayClientMessage("§cERROR: AntiBot module not found.");
        return nullptr;
    }

    std::vector<Actor*> actors= ActorUtils::getActorList(false, false);
    for (auto actor : actors)
    {
        if(mUseAntibot.mValue && antiBot->isBot(actor)) continue;
        if(mHive.mValue && actor->getmEntityIdentifier() == "hivecommon:shadow") continue;
        if(actor == player) continue;

        if(actor->mContext.mEntityId == entityId) return actor;
    }

    return nullptr;
}

static uint64_t lastAttack;

void TriggerBot::onBaseTickEvent(class BaseTickEvent &event) {
    auto player = event.mActor;
    if(!player) return;


    if (mAPSMax.mValue < mAPSMin.mValue + 1) mAPSMax.mValue = mAPSMin.mValue + 1;

    int avgAps = MathUtils::random(mAPSMin.mValue, mAPSMax.mValue);

    if (NOW - lastAttack < 1000 / avgAps) return;

    HitResult* hitResult = player->getLevel()->getHitResult();

    if(hitResult->mType == HitType::ENTITY)
    {
        Actor* actor = getActorFromEntityId(hitResult->mEntity.id);
        if(!actor) return;

        lastAttack = NOW;

        player->swing();
        player->getGameMode()->attack(actor);
    }
}