//
// Created by Flash on 12/26/2023.
//

#include "ChinaHat.h"

#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/Render.h>

#include "HUD.h"

void ChinaHat::OnRender()
{
    int pointCount = (int) Resolution.Value;
    float radius = Size.Value;

    auto corrected = Render::Transform.modelView;
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (player == nullptr) return;
    if (player->level == nullptr) return;

    for (auto actor: player->level->getRuntimeActorList())
    {
        if (actor == nullptr) continue;

        if (!actor->isPlayer() && !RenderOnMobs.Enabled) continue;
        if (actor != player && !RenderOnOthers.Enabled) continue;
        if (actor == player && RenderOnSelf.Enabled && Minecraft::ClientInstance->GetOptions()->game_thirdperson->value == 0) continue;
        Vector3 pos = actor->GetLerpedPos();

        if (player == actor)
        {
            pos = Render::Transform.playerPos;
        }

        if (actor->isPlayer() && actor != player) {
            pos.y -= 1.62f;
        }

        auto color = RGB.Enabled
                     ? HUD::GetColor(ticks)
                     : ImColor(1.f, 1.f, 1.f, Opacity.Value);
        color.Value.w = Opacity.Value;
        float height = actor->GetAABBShapeComponent()->hitboxHeight;
        Vector2 screenPos1 = {0, 0};
        Vector3 hatTip = {pos.x, pos.y + height + (float)(0.5f * Size.Value), pos.z};


        if (!corrected.OWorldToScreen(Render::Transform.origin,
                                       hatTip, screenPos1, Math::Fov,
                                       Minecraft::ClientInstance->guiData->resolution))
            continue;


        Vector2 screenPos2 = {0, 0};
        Vector2 screenPos3 = {0, 0};
        for (int i = 0; i <= pointCount; i++) {
            float angle = (i / (float) pointCount) * 360.f;
            float rad = Math::ToRadians(angle);

            float x = pos.x + radius * cosf(rad);
            float y = pos.y + height;
            float z = pos.z + radius * sinf(rad);

            Vector3 thisPos = {x, y, z};

            if (!corrected.OWorldToScreen(
                    Render::Transform.origin,
                    thisPos, screenPos3, Math::Fov,
                    Minecraft::ClientInstance->guiData->resolution))
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



                ImGui::GetBackgroundDrawList()->AddShadowConvexPoly(points, 3, color, GlowAmount.Value,
                                                                    ImVec2(1.0f, 1.0f), 0.0f);

                ImGui::GetBackgroundDrawList()->Flags |= ImDrawListFlags_AntiAliasedFill;

            }


            corrected.OWorldToScreen(Render::Transform.origin,
                                      thisPos, screenPos2, Math::Fov,
                                      Minecraft::ClientInstance->guiData->resolution);
        }
    }
}
