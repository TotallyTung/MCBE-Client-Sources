//
// Created by vastrakai on 10/4/2024.
//

#include "Tracers.hpp"

#include <Features/Modules/Misc/Friends.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Options.hpp>

void Tracers::onEnable()
{
    gFeatureManager->mDispatcher->listen<RenderEvent, &Tracers::onRenderEvent>(this);
}

void Tracers::onDisable()
{
    gFeatureManager->mDispatcher->deafen<RenderEvent, &Tracers::onRenderEvent>(this);
}
// self pasting (everywhere)
float GetDistanceBetweenPoints(const ImVec2& point1, const ImVec2& point2) {
    float dx = point1.x - point2.x;
    float dy = point1.y - point2.y;
    return sqrt(dx * dx + dy * dy);
}

float DotProduct(const ImVec2& v1, const ImVec2& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

float LengthSqr(const ImVec2& v) {
    return v.x * v.x + v.y * v.y;
}

ImVec2 getClosestPointOnLine(const std::vector<ImVec2>& points, ImVec2 imcenter) {
    ImVec2 center = ImVec2(imcenter.x, imcenter.y);
    if (points.size() == 0) return ImVec2(-1, -1);
    float minDistance = FLT_MAX;
    ImVec2 closestPoint = ImVec2(0, 0);
    for (int i = 0; i < points.size(); i++) {
        ImVec2 point1 = points[i];
        ImVec2 point2 = points[(i + 1) % points.size()];
        ImVec2 v1 = point2 - point1;
        ImVec2 v2 = center - point1;
        float dot = DotProduct(v1, v2);
        float lenSqr = LengthSqr(v1);
        float param = dot / lenSqr;
        ImVec2 pt;
        if (param < 0 || (point1.x == point2.x && point1.y == point2.y)) {
            pt = point1;
        }
        else if (param > 1) {
            pt = point2;
        }
        else {
            //pt = point1 + param * v1;
            // imvec2 doesn't have * operator
            pt.x = point1.x + param * v1.x;
            pt.y = point1.y + param * v1.y;
        }
        float distance = GetDistanceBetweenPoints(pt, center);
        if (distance < minDistance) {
            minDistance = distance;
            closestPoint = pt;
        }
    }
    return closestPoint;
}

void Tracers::onRenderEvent(RenderEvent& event)
{
    if (!ClientInstance::get()->getLevelRenderer()) return;

    auto actors = ActorUtils::getActorList(false, true);
    auto localPlayer = ClientInstance::get()->getLocalPlayer();


    auto drawList = ImGui::GetBackgroundDrawList();


    ImVec2 fromPoint;
    switch (mCenterPoint.mValue)
    {
    case CenterPoint::Top:
        fromPoint = ImVec2(ImGui::GetIO().DisplaySize.x / 2, 0);
        break;
    case CenterPoint::Center:
        fromPoint = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2);
        break;
    case CenterPoint::Bottom:
        fromPoint = ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y);
        break;
    }



    for (auto actor : actors)
    {
        if (actor == localPlayer && ClientInstance::get()->getOptions()->mThirdPerson->value == 0 && !localPlayer->getFlag<RenderCameraComponent>()) continue;
        if (actor == localPlayer && !mRenderLocal.mValue) continue;
        auto shapeComp = actor->getAABBShapeComponent();
        if (!shapeComp) continue;

        auto themeColor = ColorUtils::getThemedColor(0);

        if (actor->isPlayer())
        {
            if (gFriendManager->isFriend(actor))
            {
                if (mShowFriends.mValue) themeColor = ImColor(0.0f, 1.0f, 0.0f);
                else continue;
            }
        }

        auto points = MathUtils::getImBoxPoints(actor->getAABB());

        if (points.empty()) continue;
        ImVec2 toPoint = getClosestPointOnLine(points, fromPoint);
        drawList->AddLine(fromPoint, toPoint, themeColor, 2.0f);
    }
}
