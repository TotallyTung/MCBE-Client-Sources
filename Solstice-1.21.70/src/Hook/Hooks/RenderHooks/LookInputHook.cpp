//
// Created by vastrakai on 7/22/2024.
//

#include "LookInputHook.hpp"

#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/MinecraftSim.hpp>
#include <Features/Events/LookInputEvent.hpp>
#include <SDK/Minecraft/Options.hpp>

std::unique_ptr<Detour> LookInputHook::mDetour;

void LookInputHook::_handleLookInput(EntityContext* entityContext, CameraComponent& cameraComponent, CameraDirectLookComponent& cameraDirectLookComponent, glm::vec2 const& vec2) {
    auto original = mDetour->getOriginal<&LookInputHook::_handleLookInput>();

    if (cameraComponent.mViewName.text == "") return original(entityContext, cameraComponent, cameraDirectLookComponent, vec2);

    auto thirdPersonCamera = ptr(&cameraComponent).offset(0x120).as<CameraComponent*>();
    auto thirdPersonFront = ptr(&cameraComponent).offset(0x120 * 2).as<CameraComponent*>();
    auto deathCamera = ptr(&cameraComponent).offset(0x120 * 3).as<CameraComponent*>();
    auto freeCamera = ptr(&cameraComponent).offset(0x120 * 4).as<CameraComponent*>();

    original(entityContext, cameraComponent, cameraDirectLookComponent, vec2);

    auto holder = nes::make_holder<LookInputEvent>(entityContext, &cameraComponent, thirdPersonCamera, thirdPersonFront, deathCamera, freeCamera, &cameraDirectLookComponent, vec2);
    gFeatureManager->mDispatcher->trigger(holder);
    if (holder->isCancelled()) return;
}

void LookInputHook::init()
{
    mDetour = std::make_unique<Detour>("CameraDirectLookSystemUtil::_handleLookInput", reinterpret_cast<void*>(SigManager::CameraDirectLookSystemUtil_handleLookInput), &_handleLookInput);
}
