//
// Created by vastrakai on 7/12/2024.
//

#include "MotionBlur.hpp"

#include <Features/FeatureManager.hpp>

void MotionBlur::onEnable()
{
    gFeatureManager->mDispatcher->listen<RenderEvent, &MotionBlur::onRenderEvent, nes::event_priority::LAST>(this);
}

void MotionBlur::onDisable()
{
    gFeatureManager->mDispatcher->deafen<RenderEvent, &MotionBlur::onRenderEvent>(this);
}

void MotionBlur::onRenderEvent(RenderEvent& event)
{
    D2D::addGhostFrame(ImGui::GetForegroundDrawList(), mMaxFrames.as<int>(), mIntensity.as<float>());
}
