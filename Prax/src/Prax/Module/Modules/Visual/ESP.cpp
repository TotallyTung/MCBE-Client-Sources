//
// Created by vastrakai on 8/20/2023.
//

#include "ESP.h"
#include <src/Prax/SDK/Game/Minecraft.h>
#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/ColorUtil.h>
#include <Prax/Module/Modules/Combat/Antibot.h>
#include <Prax/Module/Modules/Player/Friends.h>
#include <Prax/Module/Modules/Player/Teams.h>
#include <Prax/SDK/Game/Entity/Components/ActorTypeComponent.h>
#include <Prax/Utils/Render/Image.h>

#include "HUD.h"
#include "Prax/SDK/Game/Entity/Components/ActorOwnerComponent.h"

void ESP::OnRender()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    for (auto&& [_, moduleOwner, type, ridc] : player->entityCtx.registry->view<ActorOwnerComponent, ActorTypeComponent, RuntimeIDComponent>().each())
    {
        Actor* actor = moduleOwner.actor;
        if (actor == player && !RenderOnSelf.Enabled) continue;

        auto color = ImColor(255, 255, 255, 255);

        if (HighlightBots.Enabled && Antibot::IsBot(actor)) color = ImColor(255, 0, 0, 255);
        else if (Antibot::IsBot(actor)) continue;
        if (Teams::IsOnTeam(actor)) continue;

        AABB aabb = actor->GetLerpedAABB();


        if (RGBMode.SelectedIndex == static_cast<int>(RGBMode::Themed) && (!Antibot::IsBot(actor) || !HighlightBots.Enabled)) {
            color = HUD::GetColor(0);
        }

        if (ShowFriends.Enabled && Friends::IsFriend(actor)) color = ImColor(0, 255, 0, 255);
        else if (!ShowFriends.Enabled && Friends::IsFriend(actor)) continue;

        ImVec4 pos = Math::Get2dDrawPoints(aabb.lower, aabb.upper);

        if (pos.x == 0 && pos.y == 0 && pos.z == 0 && pos.w == 0)
            continue;

        switch (Mode.SelectedIndex) {
        case 0: // Image
            ImGui::GetBackgroundDrawList()->AddImage(
                    (void *)Image::Images["esp.jpg"],
                    ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w));
            break;
        case 1: // 2D
            ImGui::GetBackgroundDrawList()->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(0,0,0), 0.0f, 0, 2.0f);
            ImGui::GetBackgroundDrawList()->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), color, 0.0f, 0, 1.0f);
            break;
        case 2: // 3D
            Math::DrawBox(aabb.lower,aabb.upper, 1.0f, color);
            break;
        case 3: // Outline (Draws lines to each of the outer vertices)
            Math::DrawOutlinedBox(aabb.lower, aabb.upper, 1.0f, color);
            break;
        }
    }
}
