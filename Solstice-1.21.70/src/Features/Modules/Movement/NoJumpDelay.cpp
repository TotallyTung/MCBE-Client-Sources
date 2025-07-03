//
// Created by vastrakai on 7/19/2024.
//

#include "NoJumpDelay.hpp"

#include <Features/Events/BaseTickEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>

void NoJumpDelay::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &NoJumpDelay::onBaseTickEvent>(this);

    const auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    player->getJumpControlComponent()->mNoJumpDelay = true;
}

void NoJumpDelay::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &NoJumpDelay::onBaseTickEvent>(this);

    const auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    player->getJumpControlComponent()->mNoJumpDelay = false;


}

void NoJumpDelay::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    if (!player) return;

    player->getJumpControlComponent()->mNoJumpDelay = true;
    
}
