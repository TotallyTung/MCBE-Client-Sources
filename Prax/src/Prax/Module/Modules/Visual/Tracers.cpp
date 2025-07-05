//
// Created by Flash on 9/11/2023.
//

#include "Tracers.h"
#include <src/Prax/SDK/Game/Minecraft.h>
#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/ColorUtil.h>
#include <Prax/Module/Modules/Combat/Antibot.h>
#include <Prax/Module/Modules/Player/Friends.h>
#include <Prax/SDK/Game/Render/LevelRenderer.h>
#include <Prax/Utils/Render/Render.h>

#include "HUD.h"


std::map<Actor*, Vector3> Tracers::GetHeadLocations() {
    std::map<Actor*, Vector3> locations;
    if (!Minecraft::ClientInstance->ThePlayer()) return locations;

    auto player = Minecraft::ClientInstance->ThePlayer();

    std::vector<Actor*> actors = Antibot::GetActors();

    for (Actor *actor: actors) {

        if (actor == player) continue;
        if (!actor->isAlive()) continue;

        Vector3 pos = *actor->GetRenderPositionComponent() - Vector3(0.f, 1.62f, 0.f);
        if (!actor->isPlayer())
        {
            pos = *actor->GetRenderPositionComponent();
        }

        float hitboxHeight = actor->GetAABBShapeComponent()->hitboxHeight;

        if (Mode.SelectedIndex == 2)
            pos.y += hitboxHeight;

        locations.insert(std::pair<Actor*, Vector3>(actor, pos));
    }

    return locations;
}

void Tracers::OnRender()
{
    ImVec2 screen = ImGui::GetIO().DisplaySize;
    ImVec2 center = screen;

    center.x /= 2;

    switch (Mode.SelectedIndex) {
        case 0: // Top
        // Do nothing
            break;
        case 1: // Middle
            center.y /= 2;
            break;
        case 2: // Bottom
            center.y = 0;
            break;
    }



    for (auto pos : GetHeadLocations()) {
        if (!Minecraft::ClientInstance->ThePlayer()) continue;

        auto color = ImColor(255, 255, 255, 255);
        if (RGB.SelectedIndex == 1) {
            color = HUD::GetColor(0);
        }

        if (ShowFriends.Enabled && Friends::IsFriend(pos.first)) color = ImColor(0, 255, 0, 255);
        else if (!ShowFriends.Enabled && Friends::IsFriend(pos.first)) continue;

        Vector2 screenSize = Minecraft::ClientInstance->guiData->resolution;

        // W2S
        auto points = Math::GetHexForAABB(pos.first->GetLerpedAABB());
        // assume there is lines between the points and get the closest one in said line
        Vector2 screenPos = Math::GetClosestPointOnLine(points, center);

        // Make sure screenPos is within the screen
        if (screenPos.x < 0 || screenPos.x > screenSize.x || screenPos.y < 0 || screenPos.y > screenSize.y) continue;

        ImGui::GetBackgroundDrawList()->AddLine(center, screenPos, color, 0.5f);
    }


}