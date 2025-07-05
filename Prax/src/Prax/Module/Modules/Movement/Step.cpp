//
// Created by vastrakai on 1/13/2024.
//

#include <Prax/Utils/Render/Render.h>
#include <Prax/Utils/Math.h>
#include "Step.h"

float Step::OldStep = 0;

void Step::OnEnable()
{
    if (Mode != Vanilla) return;
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (player) OldStep = player->GetMaxAutoStepComponent()->maxAutoStep;
    Logger::Write(__FUNCTION__, "step: " + std::to_string(OldStep));
}

void Step::OnDisable()
{
    if (Mode != Vanilla) return;
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (player) player->GetMaxAutoStepComponent()->maxAutoStep = OldStep;
}

// you can check if a block is air using Misc::IsAirBlock(BlockPos)

Vector3 Step::GetCollidingBlock() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return {};

    Vector3 pos = *player->getPos() - Vector3(0.f, 1.62f, 0.f);

    Vector3 blockPos = pos;

    // do not adjust the y at all for any of these blocks

    std::vector<BlockPos> collidingBlocks = std::vector<BlockPos>();

    collidingBlocks.emplace_back(blockPos.x, blockPos.y, blockPos.z);
    collidingBlocks.emplace_back(blockPos.x, blockPos.y, blockPos.z + 1);
    collidingBlocks.emplace_back(blockPos.x + 1, blockPos.y, blockPos.z);
    collidingBlocks.emplace_back(blockPos.x + 1, blockPos.y, blockPos.z + 1);

    // Sort by distance to Pos
    std::sort(collidingBlocks.begin(), collidingBlocks.end(), [pos](BlockPos a, BlockPos b) {
        return Math::Distance(pos, Vector3(a.x, a.y, a.z)) < Math::Distance(pos, Vector3(b.x, b.y, b.z));
    });

    // Return first block
    return collidingBlocks.at(0);

    return blockPos;
}

void Step::OnTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (Mode == Vanilla) {
        player->GetMaxAutoStepComponent()->maxAutoStep = StepHeight.Value;
        return;
    }

    float speed = 0.42f;

    if (player->isCollidingHorizontal() && player->GetMoveInputComponent()->forward)
    {
        // teleport up
        Vector3 pos = *player->getPos();
        pos.y = pos.y - 1.62f;

        // Teleport up
        player->GetAABBShapeComponent()->AAY += speed;
        player->GetAABBShapeComponent()->BBY += speed;
        player->GetStateVectorComponent()->velocity.y = 0.f;
    }
}

void Step::OnRender(){
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
}