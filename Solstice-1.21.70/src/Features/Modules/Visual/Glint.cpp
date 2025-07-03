//
// Created by vastrakai on 10/19/2024.
//

#include "Glint.hpp"

#include <Features/Events/RenderItemInHandDescriptionEvent.hpp>
#include <Hook/Hooks/RenderHooks/RenderItemInHandHook.hpp>


void Glint::onEnable()
{
    gFeatureManager->mDispatcher->listen<RenderItemInHandDescriptionEvent, &Glint::onRenderItemInHandDescriptionEvent>(this);
}

void Glint::onDisable()
{
    gFeatureManager->mDispatcher->deafen<RenderItemInHandDescriptionEvent, &Glint::onRenderItemInHandDescriptionEvent>(this);
}

void Glint::onRenderItemInHandDescriptionEvent(RenderItemInHandDescriptionEvent& event)
{
    float sat = mSaturation.mValue;

    ImColor color = ColorUtils::getThemedColor(0);
    event.mThis->mGlintColor = glm::vec3(color.Value.x * sat, color.Value.y * sat, color.Value.z * sat);
    event.mThis->mGlintAlpha = 1.f;
}
