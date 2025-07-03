//
// Created by vastrakai on 7/2/2024.
//

#include "NoSlowDown.hpp"

#include <Features/FeatureManager.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>

std::vector<unsigned char> gNsBytes = { 0xC3, 0x90, 0x90, 0x90 ,0x90 };
DEFINE_PATCH_FUNC(NoSlowDown::patchSlowdown, SigManager::tickEntity_ItemUseSlowdownModifierComponent, gNsBytes);

void NoSlowDown::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &NoSlowDown::onBaseTickEvent>(this);
    patchSlowdown(true);
}

void NoSlowDown::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &NoSlowDown::onBaseTickEvent>(this);
    patchSlowdown(false);
}

void NoSlowDown::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();

    auto slowdownComponent = player->getBlockMovementSlowdownMultiplierComponent();

    slowdownComponent->mBlockMovementSlowdownMultiplier = glm::vec3(0.f, 0.f, 0.f);
}