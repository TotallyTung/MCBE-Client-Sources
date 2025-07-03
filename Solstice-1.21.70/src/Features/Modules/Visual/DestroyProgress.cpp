#include "DestroyProgress.hpp"

#include <Features/FeatureManager.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Actor/GameMode.hpp>
#include <SDK/Minecraft/World/Level.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>

void DestroyProgress::onEnable()
{
    gFeatureManager->mDispatcher->listen<RenderEvent, &DestroyProgress::onRenderEvent>(this);
}

void DestroyProgress::onDisable()
{
    gFeatureManager->mDispatcher->deafen<RenderEvent, &DestroyProgress::onRenderEvent>(this);
}

void DestroyProgress::onRenderEvent(RenderEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player || ClientInstance::get()->getMouseGrabbed()) return;

    if (!player->getGameMode()) return;

    float cProgress = player->getGameMode()->mBreakProgress;
    static float lastProgress = 0.f;
    float cAnimProgress = 1.f;

    cAnimProgress = cProgress;
    if (cAnimProgress < lastProgress) lastProgress = cAnimProgress;
    cAnimProgress = MathUtils::lerp(lastProgress, cAnimProgress, ImGui::GetIO().DeltaTime * 30.f);
    lastProgress = cAnimProgress;
    cAnimProgress = MathUtils::clamp(cAnimProgress, 0.f, 1.f);

    if (0 >= cProgress || player->getLevel()->getHitResult()->mType != HitType::BLOCK) return;

    glm::ivec3 cMiningPos = player->getLevel()->getHitResult()->mBlockPos;
    auto boxSize = glm::vec3(cAnimProgress, cAnimProgress, cAnimProgress);
    glm::vec3 blockPos = cMiningPos;
    blockPos.x += 0.5f - (cAnimProgress / 2.f);
    blockPos.y += 0.5f - (cAnimProgress / 2.f);
    blockPos.z += 0.5f - (cAnimProgress / 2.f);
    auto boxAABB = AABB(blockPos, boxSize);

    ImColor cColor = ColorUtils::getThemedColor(0);
    if (mColorMode.mValue == ColorMode::Default) cColor = ImColor((int)(cAnimProgress * 255), (int)((1 - cAnimProgress) * 255), 0);
    cColor.Value.w = static_cast<int>(mOpacity.mValue * 255);


    std::vector<ImVec2> imPoints = MathUtils::getImBoxPoints(boxAABB);

    auto drawList = ImGui::GetBackgroundDrawList();

    if (mFilled.mValue) drawList->AddConvexPolyFilled(imPoints.data(), imPoints.size(), ImColor(cColor.Value.x, cColor.Value.y, cColor.Value.z, mOpacity.mValue));
    drawList->AddPolyline(imPoints.data(), imPoints.size(), cColor, 0, 2.0f);
}
