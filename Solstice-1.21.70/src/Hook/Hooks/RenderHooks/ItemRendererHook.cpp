//
// Created by jcazm on 7/27/2024.
//

#include "ItemRendererHook.hpp"
#include <SDK/Minecraft/Actor/ActorRenderData.hpp>
#include <SDK/Minecraft/Rendering/BaseActorRenderContext.hpp>
#include <Features/Events/ItemRendererEvent.hpp>

std::unique_ptr<Detour> ItemRendererHook::mDetour;

void ItemRendererHook::render(ItemRenderer *_this, BaseActorRenderContext *renderContext, ActorRenderData *actorRenderData) {
    auto original = mDetour->getOriginal<&ItemRendererHook::render>();

    auto holder = nes::make_holder<ItemRendererEvent>(_this, renderContext, actorRenderData);
    gFeatureManager->mDispatcher->trigger(holder);

    original(_this, renderContext, actorRenderData);
}

void ItemRendererHook::init() {
    mDetour = std::make_unique<Detour>("ItemRenderer::render", reinterpret_cast<void*>(SigManager::ItemRenderer_render), &render);
}
