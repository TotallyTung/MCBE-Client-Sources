//
// Created by vastrakai on 7/22/2024.
//

#include "NoCameraClip.hpp"

#include <Features/Events/LookInputEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>

void NoCameraClip::onEnable()
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    for (auto&& [id, cameraComponent] : player->mContext.mRegistry->view<CameraComponent>().each())
    {
        auto type = cameraComponent.getMode();
        if (type != CameraMode::ThirdPerson && type != CameraMode::ThirdPersonFront) continue;

        auto storage = player->mContext.mRegistry->assure_t<CameraAvoidanceComponent>();
        storage->remove(id);
    }
}

void NoCameraClip::onDisable()
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    for (auto&& [id, cameraComponent] : player->mContext.mRegistry->view<CameraComponent>().each())
    {
        auto type = cameraComponent.getMode();
        if (type != CameraMode::ThirdPerson && type != CameraMode::ThirdPersonFront) continue;

        auto storage = player->mContext.assure<CameraAvoidanceComponent>();
        storage->emplace(id, CameraAvoidanceComponent());
    }
}