//
// Created by vastrakai on 7/10/2024.
//

#include "Sprint.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>

void Sprint::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Sprint::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &Sprint::onPacketOutEvent>(this);
}

void Sprint::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Sprint::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &Sprint::onPacketOutEvent>(this);
}

void Sprint::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor; // Local player
    if (!player) return;
    auto moveInput = player->getMoveInputComponent();
    if (!moveInput) return;
    moveInput->mIsSprinting = true;
}

void Sprint::onPacketOutEvent(PacketOutEvent& event)
{
}
