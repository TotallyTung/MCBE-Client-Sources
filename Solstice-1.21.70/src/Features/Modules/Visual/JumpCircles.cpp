//
// Created by alteik on 26/10/2024.
//

#include "JumpCircles.hpp"
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Rendering/GuiData.hpp>
#include <cmath>

float JumpCircles::toRadians(float deg)
{
    return deg * (PI / 180.0f);
}

void JumpCircles::addCircle(const glm::vec3& pos, float radius, const ImVec4& color, float opacity, float glowAmount) {
    uint64_t currentTime = NOW;

    if (currentTime - lastAddTime < 100) return;

    Circle newCircle = { pos, radius, color, glowAmount, opacity, currentTime };
    circles.push_back(newCircle);
    lastAddTime = currentTime;
}

ImU32 convertToImU32(const ImVec4& color) {
    return IM_COL32(static_cast<int>(color.x * 255.0f),
                    static_cast<int>(color.y * 255.0f),
                    static_cast<int>(color.z * 255.0f),
                    static_cast<int>(color.w * 255.0f));
}

void JumpCircles::onEnable()
{
    gFeatureManager->mDispatcher->listen<RenderEvent, &JumpCircles::onRenderEvent>(this);
}

void JumpCircles::onDisable()
{
    circles.clear();
    gFeatureManager->mDispatcher->deafen<RenderEvent, &JumpCircles::onRenderEvent>(this);
}

 void JumpCircles::onRenderEvent(RenderEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player || !player->getLevel()) return;

    uint64_t currentTime = NOW;

    if (!player->wasOnGround() && player->isOnGround()) {
        glm::vec3 pos = RenderUtils::transform.mPlayerPos;
        pos.y -= 2;

        float radius = 0.1f;
        ImVec4 color = ColorUtils::getThemedColor(0);
        color.w = mOpacity.mValue;
        float glowAmount = mGlowAmount.mValue;

        addCircle(pos, radius, color, color.w, glowAmount);
    }

    for (auto it = circles.begin(); it != circles.end(); ) {
        if ((currentTime - it->startTime) > mTime.mValue) {
            it = circles.erase(it);
            continue;
        }

        // Increase radius gradually by the speed factor instead of based on elapsed time
        it->radius += mSize.mValue;

        // Opacity animation independent of elapsed time
        it->color.z = mOpacity.mValue;

        int pointCount = 100;
        glm::vec2 screenPosPrev, screenPosCurr;
        auto corrected = RenderUtils::transform.mMatrix;

        bool firstPoint = true;

        for (int i = 0; i < 16; i++) {
            ImVec4 mUIColor = it->color;

            mUIColor.z = it->opacity * (1.f - i / 16.f);

            float ringRadius = it->radius - i * ringSpacing;  // Shrinking radius per ring

            for (int j = 0; j <= pointCount; j++)
            {
                // Circle points

                float angle = (j / (float)pointCount) * 360.f;
                float rad = toRadians(angle);

                glm::vec3 currentPos = {
                    it->position.x + ringRadius * cosf(rad),
                    it->position.y,
                    it->position.z + ringRadius * sinf(rad)
                };

                if (corrected.OWorldToScreen(RenderUtils::transform.mOrigin, currentPos, screenPosCurr, MathUtils::fov,
                                         ClientInstance::get()->getGuiData()->mResolution))
                {
                    if (!firstPoint) {
                        ImGui::GetBackgroundDrawList()->AddLine(
                            ImVec2(screenPosPrev.x, screenPosPrev.y),
                            ImVec2(screenPosCurr.x, screenPosCurr.y),
                            convertToImU32(mUIColor),
                            2.0f
                        );
                    }

                    screenPosPrev = screenPosCurr;
                    firstPoint = false;
                }
            }
        }
        ++it;
    }
}