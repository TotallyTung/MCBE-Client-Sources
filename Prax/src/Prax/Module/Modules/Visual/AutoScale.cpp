//
// Created by vastrakai on 1/6/2024.
//

#include "AutoScale.h"

void AutoScale::OnTick()
{
    float scale = GuiScale.Value;

    auto guiData = Minecraft::ClientInstance->guiData;
    guiData->guiScale = scale;
    guiData->scalingMultiplier = 1 / scale;
    guiData->resolutionScaled = Vector2(guiData->resolution.x * guiData->scalingMultiplier, guiData->resolution.y * guiData->scalingMultiplier);
}

Vector2 AutoScale::OldScaledResolution = Vector2(0, 0);
float AutoScale::OldScaleMultiplier = 0;
float AutoScale::OldGuiScale = 0;

void AutoScale::OnEnable()
{
    float scale = GuiScale.Value;

    auto guiData = Minecraft::ClientInstance->guiData;
    OldGuiScale = guiData->guiScale;
    OldScaleMultiplier = guiData->scalingMultiplier;
    OldScaledResolution = guiData->resolutionScaled;
    guiData->guiScale = scale;
    guiData->scalingMultiplier = 1 / scale;
    guiData->resolutionScaled = Vector2(guiData->resolution.x * guiData->scalingMultiplier, guiData->resolution.y * guiData->scalingMultiplier);
}

void AutoScale::OnDisable()
{
    auto guiData = Minecraft::ClientInstance->guiData;
    guiData->guiScale = OldGuiScale;
    guiData->scalingMultiplier = OldScaleMultiplier;
    guiData->resolutionScaled = OldScaledResolution;
}
