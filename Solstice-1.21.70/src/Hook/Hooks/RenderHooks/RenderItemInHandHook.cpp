//
// Created by vastrakai on 10/19/2024.
//

#include "RenderItemInHandHook.hpp"
#include <Features/Events/RenderItemInHandDescriptionEvent.hpp>

std::unique_ptr<Detour> RenderItemInHandHook::mDetour;

void* RenderItemInHandHook::RenderItemInHandDescriptionCtor(RenderItemInHandDescription* _this, void* renderObject, void* itemFlags,
    void* material, void* glintTexture, void* worldMatrix, bool isDrawingUI, void* globalConstantBuffers,
    unsigned short viewId, void* renderMetadata)
{
    auto original = mDetour->getOriginal<&RenderItemInHandDescriptionCtor>();
    auto result = original(_this, renderObject, itemFlags, material, glintTexture, worldMatrix, isDrawingUI, globalConstantBuffers, viewId, renderMetadata);

    nes::event_holder<RenderItemInHandDescriptionEvent> holder = nes::make_holder<RenderItemInHandDescriptionEvent>(_this, renderObject, itemFlags, material, glintTexture, worldMatrix, isDrawingUI, globalConstantBuffers);
    gFeatureManager->mDispatcher->trigger(holder);
    if (holder->isCancelled()) return nullptr;

    return result;
}

void RenderItemInHandHook::init()
{
    mDetour = std::make_unique<Detour>("mce::framebuilder::RenderItemInHandDescription::RenderItemInHandDescription",
        reinterpret_cast<void*>(SigManager::mce_framebuilder_RenderItemInHandDescription_ctor), &RenderItemInHandDescriptionCtor);
    mDetour->enable();
}
