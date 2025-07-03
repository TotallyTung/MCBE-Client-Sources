//
// Created by alteik on 11/10/2024.
//

#include "ChinaHat.hpp"
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Rendering/GuiData.hpp>
#include <SDK/Minecraft/Options.hpp>
#include <SDK/Minecraft/Rendering/GuiData.hpp>

float ChinaHat::toRadians(float deg) {
    return deg * (PI / 180.0f);
}

void ChinaHat::onEnable() {
    gFeatureManager->mDispatcher->listen<RenderEvent, &ChinaHat::onRenderEvent>(this);
}

void ChinaHat::onDisable() {
    gFeatureManager->mDispatcher->deafen<RenderEvent, &ChinaHat::onRenderEvent>(this);
}

void ChinaHat::onRenderEvent(RenderEvent& event) {
    if (ClientInstance::get()->getOptions()->mThirdPerson->value == 0) return;

    int pointCount = 20;
    float radius = mSize.mValue;

    auto corrected = RenderUtils::transform.mMatrix;
    auto player = ClientInstance::get()->getLocalPlayer();

    if (!player || !player->getLevel()) return;

    glm::vec3 pos = RenderUtils::transform.mPlayerPos;
    pos.y -= 1.62;

    auto color = ColorUtils::getThemedColor(0);
    color.Value.w = mOpacity.mValue;
    float height = player->getAABBShapeComponent()->mHeight;
    glm::vec2 screenPos1 = {0, 0};
    glm::vec3 hatTip = {pos.x, pos.y + height + (0.5f * mSize.mValue), pos.z};

    if (!corrected.OWorldToScreen(RenderUtils::transform.mOrigin, hatTip, screenPos1, MathUtils::fov,
                                  ClientInstance::get()->getGuiData()->mResolution))
        return;

    glm::vec2 screenPos2 = {0, 0};
    glm::vec2 screenPos3 = {0, 0};
    for (int i = 0; i <= pointCount; i++) {
        float angle = (i / (float) pointCount) * 360.f;
        float rad = toRadians(angle);

        float x = pos.x + radius * cosf(rad);
        float y = pos.y + height;
        float z = pos.z + radius * sinf(rad);

        glm::vec3 thisPos = {x, y, z};

        if (!corrected.OWorldToScreen(RenderUtils::transform.mOrigin, thisPos, screenPos3, MathUtils::fov,
                                      ClientInstance::get()->getGuiData()->mResolution))
            continue;

        if (i > 0 && (screenPos2.x != 0 && screenPos2.y != 0)) {
            ImGui::GetBackgroundDrawList()->Flags &= ~ImDrawListFlags_AntiAliasedFill;

            ImGui::GetBackgroundDrawList()->AddTriangleFilled(
                    ImVec2(screenPos1.x, screenPos1.y),
                    ImVec2(screenPos2.x, screenPos2.y),
                    ImVec2(screenPos3.x, screenPos3.y),
                    color);
            ImVec2 points[3] = {screenPos1,
                                screenPos2,
                                screenPos3};

            ImGui::GetBackgroundDrawList()->AddShadowConvexPoly(points, 3, color, mGlowAmount.mValue,
                                                                ImVec2(1.0f, 1.0f), 0.0f);

            ImGui::GetBackgroundDrawList()->Flags |= ImDrawListFlags_AntiAliasedFill;

        }

        corrected.OWorldToScreen(RenderUtils::transform.mOrigin,
                                 thisPos, screenPos2, MathUtils::fov,
                                 ClientInstance::get()->getGuiData()->mResolution);
    }
}