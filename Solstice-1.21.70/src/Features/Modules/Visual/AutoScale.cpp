//
// Created by vastrakai on 6/30/2024.
//

#include "AutoScale.hpp"

#include <Features/FeatureManager.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Rendering/GuiData.hpp>

void AutoScale::onEnable()
{
    // Store the old values
    auto guiData = ClientInstance::get()->getGuiData();
    mOldGuiScale = guiData->mGuiScale;
    mOldScaleMultiplier = guiData->mScalingMultiplier;
    mOldScaledResolution = guiData->mResolutionScaled;

    // Set the new values
    guiData->mGuiScale = mScaleSetting.mValue;
    guiData->mScalingMultiplier = 1 / mScaleSetting.mValue;
    guiData->mResolutionScaled = glm::vec2(guiData->mResolution.x * guiData->mScalingMultiplier, guiData->mResolution.y * guiData->mScalingMultiplier);

    gFeatureManager->mDispatcher->listen<RenderEvent, &AutoScale::onRenderEvent>(this);
}

void AutoScale::onDisable()
{
    gFeatureManager->mDispatcher->deafen<RenderEvent, &AutoScale::onRenderEvent>(this);

    // Restore the old values
    auto guiData = ClientInstance::get()->getGuiData();
    guiData->mGuiScale = mOldGuiScale;
    guiData->mScalingMultiplier = mOldScaleMultiplier;
    guiData->mResolutionScaled = mOldScaledResolution;
}

void AutoScale::onRenderEvent(RenderEvent& event)
{
    float scale = mScaleSetting.mValue;
    auto guiData = ClientInstance::get()->getGuiData();
    guiData->mGuiScale = scale;
    guiData->mScalingMultiplier = 1 / scale;
    guiData->mResolutionScaled = glm::vec2(guiData->mResolution.x * guiData->mScalingMultiplier, guiData->mResolution.y * guiData->mScalingMultiplier);
}
