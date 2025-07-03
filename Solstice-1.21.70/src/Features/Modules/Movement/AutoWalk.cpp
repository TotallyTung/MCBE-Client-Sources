//
// Created by dark on 3/12/2025.
//

#include "AutoWalk.hpp"

#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/RenderEvent.hpp>

void AutoWalk::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &AutoWalk::onBaseTickEvent>(this);
}

void AutoWalk::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &AutoWalk::onBaseTickEvent>(this);
}

void AutoWalk::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    if (!player) return;
}

void AutoWalk::onRenderEvent(RenderEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;
}