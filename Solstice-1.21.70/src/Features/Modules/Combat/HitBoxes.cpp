//
// Created by dark on 02/19/2025.
//

#include "HitBoxes.hpp"
#include <Features/Events/BaseTickEvent.hpp>
#include <Utils/GameUtils/ActorUtils.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <Features/Modules/Misc/AntiBot.hpp>

void HitBoxes::onEnable() {
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &HitBoxes::onBaseTickEvent>(this);
}

void HitBoxes::onDisable() {
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &HitBoxes::onBaseTickEvent>(this);

    auto actors = ActorUtils::getActorList(true, false);
    auto localPlayer = ClientInstance::get()->getLocalPlayer();

    if (!localPlayer)
        return;

    auto lpShape = localPlayer->getAABBShapeComponent();
    if (!lpShape)
        return;

    for (auto actor : actors) {
        if (actor == localPlayer)
            continue;
        auto shape = actor->getAABBShapeComponent();
        if (!shape)
            continue;

        shape->mHeight = lpShape->mHeight;
        shape->mWidth = lpShape->mWidth;
    }
}

void HitBoxes::onBaseTickEvent(class BaseTickEvent& event) {
    auto actors = ActorUtils::getActorList(true, false);
    auto localPlayer = ClientInstance::get()->getLocalPlayer();

    if (!localPlayer)
        return;

    for (auto actor : actors) {
        if (actor == localPlayer)
            continue;
        auto shape = actor->getAABBShapeComponent();
        if (!shape)
            continue;

        shape->mHeight = mHeight.mValue;
        shape->mWidth = mWidth.mValue;
    }
}