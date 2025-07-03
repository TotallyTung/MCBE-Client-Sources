//
// Created by vastrakai on 7/8/2024.
//

#include "ActorRenderDispatcherHook.hpp"
#include <Features/Events/ActorRenderEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>

#include "D3DHook.hpp"

std::unique_ptr<Detour> ActorRenderDispatcherHook::mDetour;

void ActorRenderDispatcherHook::render(ActorRenderDispatcher* _this, BaseActorRenderContext* entityRenderContext,
    Actor* entity, glm::vec3* cameraTargetPos, glm::vec3* pos, glm::vec2* rot, bool ignoreLighting)
{
    auto oFunc = mDetour->getOriginal<&render>();
    auto localPlayer = ClientInstance::get()->getLocalPlayer();
    if (!localPlayer) return;

    auto holder = nes::make_holder<ActorRenderEvent>(_this, entityRenderContext, entity, cameraTargetPos, pos, rot, ignoreLighting, mDetour.get());
    gFeatureManager->mDispatcher->trigger(holder);
    if (holder->isCancelled())
    {
        return;
    }

    return oFunc(_this, entityRenderContext, entity, cameraTargetPos, pos, rot, ignoreLighting);
}

void ActorRenderDispatcherHook::init()
{
    mDetour = std::make_unique<Detour>("ActorRenderDispatcher::render", reinterpret_cast<void*>(SigManager::ActorRenderDispatcher_render), &ActorRenderDispatcherHook::render);
}
