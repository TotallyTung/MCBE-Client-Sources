//
// Created by vastrakai on 12/25/2023.
//

#include "Guiscale.h"

void Guiscale::OnRun(std::vector<std::string> args)
{
    if (args.size() < 1) {
        AddCommandMessage("Usage: .guiscale <scale>");
        return;
    }

    try {
        std::stof(args[0]);
    } catch (...) {
        // Print out all the values
        AddCommandMessage("Usage: .guiscale <scale>");
        AddCommandMessage("Invalid scale");
        return;
    }

    float scale = std::stof(args[0]);

    auto guiData = Minecraft::ClientInstance->guiData;
    guiData->guiScale = scale;
    guiData->scalingMultiplier = 1 / scale;
    guiData->resolutionScaled = Vector2(guiData->resolution.x * guiData->scalingMultiplier, guiData->resolution.y * guiData->scalingMultiplier);

    AddCommandMessage("Successfully set gui scale to " + std::to_string(scale));
}
