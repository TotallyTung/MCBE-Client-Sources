//
// Created by Flash on 10/15/2023.
//

#pragma once

#include <Prax/SDK/Game/Misc/Structs.h>
#include <Prax/SDK/Game/Minecraft.h>

class LevelRenderPlayer {
public:
    BUILD_ACCESS(this, glm::vec3, cameraPos, 0x5FC);
    BUILD_ACCESS(this, float, fovMultiplier, 0xD78);
    BUILD_ACCESS(this, float, lastFovMultiplier, 0xD7C);
};

class LevelRenderer
{
public:
    BUILD_ACCESS(this, LevelRenderPlayer*, levelRendererPlayer, 0x308);
};