//
// Created by Flash on 7/11/2023.
//

#include "SelectedBlock.h"

#include <Prax/Utils/Math.h>
#include <Prax/SDK/Game/Render/LevelRenderer.h>
void SelectedBlock::OnRender() {
    // Get the position of selected block
    if (!Minecraft::ClientInstance->ThePlayer())
        return;

    // Check if imgui is Initialized
    if (!Math::DrawList)
        return;

    BlockPos blockPos = Minecraft::ClientInstance->ThePlayer()->level->getHitResult()->blockPos;

    if (blockPos.x == 0 && blockPos.y == 0 && blockPos.z == 0)
        return;

    AABB aabb = AABB(Vector3(blockPos.x, blockPos.y, blockPos.z), Vector3(1, 1, 1));

    if (!Minecraft::ClientInstance->levelRenderer->levelRendererPlayer) return;
    Math::DrawOutlinedBox(aabb.lower, aabb.upper, 1.0f, ImColor(255, 255, 255, 255));
}
