//
// Created by vastrakai on 9/13/2024.
//

#include "FullBright.hpp"

#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Options.hpp>

void FullBright::onEnable()
{
    auto ci = ClientInstance::get();
    auto options = ci->getOptions();
    auto gamma = options->mGfxGamma;
}

void FullBright::onDisable()
{
    auto ci = ClientInstance::get();
    auto options = ci->getOptions();
    auto gamma = options->mGfxGamma;
}

void FullBright::onRenderEvent(RenderEvent& event)
{
    static float currentGamma = 1.f;
    static float speed = 10.f;
    float targetGamma = mEnabled ? 13.f : 1.f;

    // (if the current gamma is within 0.001 of the target gamma)
    mFinishedAnimation = std::abs(currentGamma - targetGamma) < 0.001f;
    if (mFinishedAnimation && !mEnabled) return;

    float delta = ImGui::GetIO().DeltaTime;

    currentGamma = std::lerp(currentGamma, targetGamma, speed * delta);

    ClientInstance::get()->getOptions()->mGfxGamma->mValue = currentGamma;
}
