//
// Created by Flash on 7/11/2023.
//

#include <src/Prax/SDK/Game/Minecraft.h>
#include "Math.h"

#include "Render/Render.h"
#define PI       3.14159265358979323846

Vector2 Math::Fov;
ImColor Math::Color;
Vector3 Math::Origin;
Vector2 Math::ScreenSize;
ImDrawList* Math::DrawList;


float Math::EaseInOutExpo(float pct)
{
    if (pct < 0.5f) {
        return (pow(2.f, 16.f * pct) - 1.f) / 510.f;
    }
    else {
        return 1.f - 0.5f * pow(2.f, -16.f * (pct - 0.5f));
    }
}

bool Math::DrawBox(Vector3 lower, Vector3 upper, float lineWidth) {
    if (Fov == 0.0f) return false;
    if (Color.Value.w == 0.0f) return false;
    if (!DrawList) return false;
    if (!Minecraft::ClientInstance->ThePlayer()) return false;

    auto corrected = Render::Transform.modelView;
    Vector3 levelOrigin = Origin;
    Vector2 fov = Fov;
    Vector2 screenSize = Minecraft::ClientInstance->guiData->resolution;

    Vector3 cornerA = lower;
    Vector3 cornerH = upper;
    Vector3 cornerB = { cornerH.x, cornerA.y, cornerA.z };
    Vector3 cornerC = { cornerA.x, cornerA.y, cornerH.z };
    Vector3 cornerD = { cornerH.x, cornerA.y, cornerH.z };
    Vector3 cornerE = { cornerA.x, cornerH.y, cornerA.z };
    Vector3 cornerF = { cornerH.x, cornerH.y, cornerA.z };
    Vector3 cornerG = { cornerA.x, cornerH.y, cornerH.z };

    Vector2 screenPosA = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerA, screenPosA, fov, screenSize)) {
        return false;
    }
    Vector2 screenPosB = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerB, screenPosB, fov, screenSize)) {
        return false;
    }
    Vector2 screenPosC = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerC, screenPosC, fov, screenSize)) {
        return false;
    }
    Vector2 screenPosD = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerD, screenPosD, fov, screenSize)) {
        return false;
    }
    Vector2 screenPosE = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerE, screenPosE, fov, screenSize)) {
        return false;
    }
    Vector2 screenPosF = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerF, screenPosF, fov, screenSize)) {
        return false;
    }
    Vector2 screenPosG = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerG, screenPosG, fov, screenSize)) {
        return false;
    }
    Vector2 screenPosH = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerH, screenPosH, fov, screenSize)) {
        return false;
    }

    //Bottom face
    DrawList->AddLine(screenPosA, screenPosB, Color, lineWidth);
    DrawList->AddLine(screenPosB, screenPosD, Color, lineWidth);
    DrawList->AddLine(screenPosC, screenPosD, Color, lineWidth);
    DrawList->AddLine(screenPosC, screenPosA, Color, lineWidth);

    //Top face
    DrawList->AddLine(screenPosE, screenPosF, Color, lineWidth);
    DrawList->AddLine(screenPosF, screenPosH, Color, lineWidth);
    DrawList->AddLine(screenPosG, screenPosH, Color, lineWidth);
    DrawList->AddLine(screenPosG, screenPosE, Color, lineWidth);

    //Corners to connect bottom and top
    DrawList->AddLine(screenPosA, screenPosE, Color, lineWidth);
    DrawList->AddLine(screenPosB, screenPosF, Color, lineWidth);
    DrawList->AddLine(screenPosC, screenPosG, Color, lineWidth);
    DrawList->AddLine(screenPosD, screenPosH, Color, lineWidth);

    return true;
}

bool Math::DrawBox(Vector3 lower, Vector3 upper, float lineWidth, ImColor color) {
    if (Fov == 0.0f) return false;
    if (Color.Value.w == 0.0f) return false;
    if (!DrawList) return false;
    if (!Minecraft::ClientInstance->ThePlayer()) return false;

    auto corrected = Render::Transform.modelView;
    Vector3 levelOrigin = Origin;
    Vector2 fov = Fov;
    Vector2 screenSize = Minecraft::ClientInstance->guiData->resolution;

    Vector3 cornerA = lower;
    Vector3 cornerH = upper;
    Vector3 cornerB = { cornerH.x, cornerA.y, cornerA.z };
    Vector3 cornerC = { cornerA.x, cornerA.y, cornerH.z };
    Vector3 cornerD = { cornerH.x, cornerA.y, cornerH.z };
    Vector3 cornerE = { cornerA.x, cornerH.y, cornerA.z };
    Vector3 cornerF = { cornerH.x, cornerH.y, cornerA.z };
    Vector3 cornerG = { cornerA.x, cornerH.y, cornerH.z };

    Vector2 screenPosA = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerA, screenPosA, fov, screenSize)) return false;

    Vector2 screenPosB = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerB, screenPosB, fov, screenSize))return false;

    Vector2 screenPosC = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerC, screenPosC, fov, screenSize))return false;

    Vector2 screenPosD = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerD, screenPosD, fov, screenSize))return false;

    Vector2 screenPosE = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerE, screenPosE, fov, screenSize))return false;

    Vector2 screenPosF = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerF, screenPosF, fov, screenSize))return false;

    Vector2 screenPosG = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerG, screenPosG, fov, screenSize)) return false;

    Vector2 screenPosH = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerH, screenPosH, fov, screenSize)) return false;

    //Bottom face
    DrawList->AddLine(screenPosA, screenPosB, color, lineWidth);
    DrawList->AddLine(screenPosB, screenPosD, color, lineWidth);
    DrawList->AddLine(screenPosC, screenPosD, color, lineWidth);
    DrawList->AddLine(screenPosC, screenPosA, color, lineWidth);

    //Top face
    DrawList->AddLine(screenPosE, screenPosF, color, lineWidth);
    DrawList->AddLine(screenPosF, screenPosH, color, lineWidth);
    DrawList->AddLine(screenPosG, screenPosH, color, lineWidth);
    DrawList->AddLine(screenPosG, screenPosE, color, lineWidth);

    //Corners to connect bottom and top
    DrawList->AddLine(screenPosA, screenPosE, color, lineWidth);
    DrawList->AddLine(screenPosB, screenPosF, color, lineWidth);
    DrawList->AddLine(screenPosC, screenPosG, color, lineWidth);
    DrawList->AddLine(screenPosD, screenPosH, color, lineWidth);

    return true;
}

bool Math::DrawBoxFilled(Vector3 lower, Vector3 upper, float lineWidth, ImColor borderColor, ImColor filledColor, bool renderEdges) {
    if (Fov == 0.0f) return false;
    if (Color.Value.w == 0.0f) return false;
    if (!DrawList) return false;
    if (!Minecraft::ClientInstance->ThePlayer()) return false;

    auto corrected = Render::Transform.modelView;
    Vector3 levelOrigin = Origin;
    Vector2 fov = Fov;
    Vector2 screenSize = Minecraft::ClientInstance->guiData->resolution;

    Vector3 cornerA = lower;
    Vector3 cornerH = upper;
    Vector3 cornerB = { cornerH.x, cornerA.y, cornerA.z };
    Vector3 cornerC = { cornerA.x, cornerA.y, cornerH.z };
    Vector3 cornerD = { cornerH.x, cornerA.y, cornerH.z };
    Vector3 cornerE = { cornerA.x, cornerH.y, cornerA.z };
    Vector3 cornerF = { cornerH.x, cornerH.y, cornerA.z };
    Vector3 cornerG = { cornerA.x, cornerH.y, cornerH.z };

    Vector2 screenPosA = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerA, screenPosA, fov, screenSize)) return false;

    Vector2 screenPosB = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerB, screenPosB, fov, screenSize))return false;

    Vector2 screenPosC = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerC, screenPosC, fov, screenSize))return false;

    Vector2 screenPosD = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerD, screenPosD, fov, screenSize))return false;

    Vector2 screenPosE = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerE, screenPosE, fov, screenSize))return false;

    Vector2 screenPosF = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerF, screenPosF, fov, screenSize))return false;

    Vector2 screenPosG = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerG, screenPosG, fov, screenSize)) return false;

    Vector2 screenPosH = { 0,0 };
    if (!corrected.OWorldToScreen(levelOrigin, cornerH, screenPosH, fov, screenSize)) return false;

    //Bottom face
    DrawList->AddLine(screenPosA, screenPosB, borderColor, lineWidth);
    DrawList->AddLine(screenPosB, screenPosD, borderColor, lineWidth);
    DrawList->AddLine(screenPosC, screenPosD, borderColor, lineWidth);
    DrawList->AddLine(screenPosC, screenPosA, borderColor, lineWidth);

    //Top face
    DrawList->AddLine(screenPosE, screenPosF, borderColor, lineWidth);
    DrawList->AddLine(screenPosF, screenPosH, borderColor, lineWidth);
    DrawList->AddLine(screenPosG, screenPosH, borderColor, lineWidth);
    DrawList->AddLine(screenPosG, screenPosE, borderColor, lineWidth);

    //Corners to connect bottom and top
    DrawList->AddLine(screenPosA, screenPosE, borderColor, lineWidth);
    DrawList->AddLine(screenPosB, screenPosF, borderColor, lineWidth);
    DrawList->AddLine(screenPosC, screenPosG, borderColor, lineWidth);
    DrawList->AddLine(screenPosD, screenPosH, borderColor, lineWidth);

    // Fill in all 6 faces
    DrawList->AddQuadFilled(screenPosA, screenPosB, screenPosD, screenPosC, filledColor);
    DrawList->AddQuadFilled(screenPosE, screenPosF, screenPosH, screenPosG, filledColor);
    DrawList->AddQuadFilled(screenPosA, screenPosC, screenPosG, screenPosE, filledColor);
    DrawList->AddQuadFilled(screenPosB, screenPosD, screenPosH, screenPosF, filledColor);
    DrawList->AddQuadFilled(screenPosA, screenPosB, screenPosF, screenPosE, filledColor);
    DrawList->AddQuadFilled(screenPosC, screenPosD, screenPosH, screenPosG, filledColor);


    return true;
}

ImVec4 Math::Get2dDrawPoints(Vector3 lower, Vector3 upper) {
    if (Fov == 0.0f) return ImVec4();
    if (Color.Value.w == 0.0f) return ImVec4();
    if (!DrawList) return ImVec4();
    if (!Minecraft::ClientInstance->ThePlayer()) return ImVec4();

    auto corrected = Render::Transform.modelView;
    Vector3 levelOrigin = Origin;
    Vector2 fov = Fov;
    Vector2 screenSize = Minecraft::ClientInstance->guiData->resolution;

    AABB aabb = AABB(lower, upper, true);

    return corrected.getRectForAABB(aabb, levelOrigin, fov, screenSize);
}

// ported from PraxExternal
std::vector<Vector2> Math::GetHexForAABB(AABB aabb) {
    if (Fov == 0.0f || Color.Value.w == 0.0f || !DrawList || !Minecraft::ClientInstance->ThePlayer())
        return {};

    auto corrected = Render::Transform.modelView;
    Vector3 origin = Origin;
    Vector2 fov = Fov;
    Vector2 displaySize = Minecraft::ClientInstance->guiData->resolution;

    Vector3 worldPoints[8];

    worldPoints[0] = Vector3(aabb.lower.x, aabb.lower.y, aabb.lower.z);
    worldPoints[1] = Vector3(aabb.lower.x, aabb.lower.y, aabb.upper.z);
    worldPoints[2] = Vector3(aabb.upper.x, aabb.lower.y, aabb.lower.z);
    worldPoints[3] = Vector3(aabb.upper.x, aabb.lower.y, aabb.upper.z);
    worldPoints[4] = Vector3(aabb.lower.x, aabb.upper.y, aabb.lower.z);
    worldPoints[5] = Vector3(aabb.lower.x, aabb.upper.y, aabb.upper.z);
    worldPoints[6] = Vector3(aabb.upper.x, aabb.upper.y, aabb.lower.z);
    worldPoints[7] = Vector3(aabb.upper.x, aabb.upper.y, aabb.upper.z);

    // Convert to screen coord
    std::vector<Vector2> points;
    for (int i = 0; i < 8; i++) {
        Vector2 result;
        if (!corrected.OWorldToScreen(origin, worldPoints[i], result, fov, displaySize))
            return {};
        if (result.x < 0 || result.y < 0 || result.x > displaySize.x || result.y > displaySize.y)
            return {};
        if (result != Vector2(FLT_MAX, FLT_MAX))
            points.push_back(result);
        else
            return {};
    }
    if (points.size() < 3)
        return {};

    // Find start vertex
    auto startIt = std::min_element(points.begin(), points.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.x < rhs.x;
    });
    Vector2 start = *startIt;

    // Follow outer line
    std::vector<Vector2> indices;
    Vector2 current = start;
    indices.push_back(current);
    float lastDirAtan2 = atan2(0, -1);
    do {
        float smallestAngle = PI * 2;
        Vector2 smallestDir;
        Vector2 smallestE;
        for (const auto& t : points) {
            if (current == t)
                continue;

            // Angle between vectors
            Vector2 dir = t - current;
            float angle = atan2(dir.y, dir.x) - lastDirAtan2;
            if (angle > PI)
                angle -= PI * 2;
            else if (angle <= -PI)
                angle += PI * 2;
            if (angle >= 0 && angle < smallestAngle) {
                smallestAngle = angle;
                smallestDir = dir;
                smallestE = t;
            }
        }

        if (smallestE.x != 0 && smallestE.y != 0)
            indices.push_back(smallestE);
        lastDirAtan2 = atan2(smallestDir.y, smallestDir.x);
        current = smallestE;
    } while (current != start && indices.size() < 8);

    return indices;
}

bool Math::DrawOutlinedBox(Vector3 lower, Vector3 upper, float x, ImColor color)
{
    if (Fov == 0.0f) return false;
    if (Color.Value.w == 0.0f) return false;
    if (!DrawList) return false;
    if (!Minecraft::ClientInstance->ThePlayer()) return false;

    Vector3 levelOrigin = Origin;
    Vector2 fov = Fov;
    Vector2 screenSize = Minecraft::ClientInstance->guiData->resolution;

    // Use GetHexForAABB to get the points of the hexagon
    std::vector<Vector2> points = GetHexForAABB(AABB(lower, upper, true));

    // Draw lines between each of the points
    for (int i = 0; i < points.size(); i++) {
        DrawList->AddLine(points[i], points[(i + 1) % points.size()], color, x);
    }

    return true;
}

bool Math::DrawOutlinedBoxNaCl(Vector3 lower, Vector3 upper, float x, ImColor color)
{
    if (Fov == 0.0f) return false;
    if (Color.Value.w == 0.0f) return false;
    if (!Minecraft::ClientInstance->ThePlayer()) return false;

    std::vector<Vector2> points = GetHexForAABB(AABB(lower, upper, true));

    const auto fgList = ImGui::GetForegroundDrawList();
    const auto bgList = ImGui::GetBackgroundDrawList();

    Vector2 skipped = points[0];

    for (int i = 0; i < points.size(); i++) {
        if (i % 3 == 0) {
            skipped = points[i];
        } else if (i % 3 == 1) {
            Render::PushDrawList(fgList);
            Render::DrawGlowLine(skipped, points[(i + 1) % points.size()], color, x);
            Render::PushDrawList(bgList);
            Render::DrawGlowLine(skipped, points[(i + 1) % points.size()], color, x);
        } else {
            Render::PushDrawList(fgList);
            Render::DrawGlowLine(points[i], points[(i + 1) % points.size()], color, x);
            Render::PushDrawList(bgList);
            Render::DrawGlowLine(points[i], points[(i + 1) % points.size()], color, x);
        }
    }

    Render::PopDrawList();

    return true;
}

float Math::GetDistance(ImVec2 a, ImVec2 b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

float Math::GetDistanceBetweenPoints(Vector2 value, ImVec2 center)
{
    return sqrt(pow(value.x - center.x, 2) + pow(value.y - center.y, 2));
}

// This function is used to get the closest point on a line to a center point
float GetDistanceBetweenPoints(const Vector2& point1, const Vector2& point2) {
    float dx = point1.x - point2.x;
    float dy = point1.y - point2.y;
    return sqrt(dx * dx + dy * dy);
}

float DotProduct(const Vector2& v1, const Vector2& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

float LengthSqr(const Vector2& v) {
    return v.x * v.x + v.y * v.y;
}

Vector2 Math::GetClosestPointOnLine(const std::vector<Vector2>& points, ImVec2 imcenter) {
    Vector2 center = Vector2(imcenter.x, imcenter.y);
    if (points.size() == 0) return Vector2(-1, -1);
    float minDistance = FLT_MAX;
    Vector2 closestPoint = Vector2(0, 0);
    for (int i = 0; i < points.size(); i++) {
        Vector2 point1 = points[i];
        Vector2 point2 = points[(i + 1) % points.size()];
        Vector2 v1 = point2 - point1;
        Vector2 v2 = center - point1;
        float dot = DotProduct(v1, v2);
        float lenSqr = LengthSqr(v1);
        float param = dot / lenSqr;
        Vector2 pt;
        if (param < 0 || (point1.x == point2.x && point1.y == point2.y)) {
            pt = point1;
        }
        else if (param > 1) {
            pt = point2;
        }
        else {
            pt = point1 + param * v1;
        }
        float distance = GetDistanceBetweenPoints(pt, center);
        if (distance < minDistance) {
            minDistance = distance;
            closestPoint = pt;
        }
    }
    return closestPoint;
}

bool Math::LineIntersection(ImVec2 start, ImVec2 end, ImVec2 start2, ImVec2 end2, ImVec2& point)
{
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = end.x - start.x;     s1_y = end.y - start.y;
    s2_x = end2.x - start2.x;     s2_y = end2.y - start2.y;

    float s, t;
    s = (-s1_y * (start.x - start2.x) + s1_x * (start.y - start2.y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = (s2_x * (start.y - start2.y) - s2_y * (start.x - start2.x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        point.x = start.x + (t * s1_x);
        point.y = start.y + (t * s1_y);
        return true;
    }

    return false; // No collision
}

float Math::DegToRad(float x)
{
    return x * (PI / 180.0f);
}

float Math::Round(float bps, int decimals)
{
    float p = pow(10, decimals);
    bps = bps * p;
    float tmp = round(bps);
    return tmp / p;
}

float Math::SnapYaw(float x)
{
    // snap to a cardinal direction (yaw range: -180 to 180)
    if (x < -135 || x > 135) return -180;
    if (x < -45) return -90;
    if (x < 45) return 0;
    if (x < 135) return 90;
    return 180;
}

bool Math::DrawFilledOutlinedBox(Vector3 lower, Vector3 upper, float x, ImColor color, ImColor outlineColor)
{
    if (Fov == 0.0f) return false;
    if (Color.Value.w == 0.0f) return false;
    if (!DrawList) return false;
    if (!Minecraft::ClientInstance->ThePlayer()) return false;

    Vector3 levelOrigin = Origin;
    Vector2 fov = Fov;
    Vector2 screenSize = Minecraft::ClientInstance->guiData->resolution;

    // Use GetHexForAABB to get the points of the hexagon
    std::vector<Vector2> points = GetHexForAABB(AABB(lower, upper, true));

    // Draw lines between each of the points
    for (int i = 0; i < points.size(); i++) {
        DrawList->AddLine(points[i], points[(i + 1) % points.size()], outlineColor, x);
    }

    // Fill in the area between lines (Use a rectangle + clipper to fill in the area)
    DrawList->AddRectFilled(points[0], points[2], color);
    DrawList->AddRectFilled(points[3], points[5], color);
    DrawList->AddRectFilled(points[6], points[1], color);
    DrawList->AddRectFilled(points[7], points[4], color);


    return true;
}


void Math::DrawQuad(ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4, float lineWidth) {
    DrawList->AddQuad(p1, p2, p3, p4, Color, lineWidth);
}

void Math::DrawQuad(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float lineWidth) {
    DrawList->AddQuad(p1, p2, p3, p4, Color, lineWidth);
}

void Math::DrawLine(Vector2 start, Vector2 end, float lineWidth) {
    DrawList->AddLine(start, end, Color, lineWidth);
}


ImVec2 Math::Lerp(ImVec2 a, ImVec2 b, float t) {
    return ImVec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
}

// Start, End, Value
float Math::Lerp(float a, float b, float c) {
    return a + c * (b - a);
}

Vector3 Math::Lerp(Vector3 a, Vector3 b, float t) {
    return Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
}

float Math::Animate(float current, float endPoint, float speed) {
    speed = (speed < 0.0) ? 0.0 : ((speed > 1.0) ? 1.0 : speed);
    float dif = fmax(endPoint, current) - fmin(endPoint, current);
    float factor = dif * speed;
    return current + (endPoint > current ? factor : -factor);
}

float Math::ToRadians(float deg) {
    return deg * (PI / 180.0f);
}

int Math::RandomInt(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

float Math::Clamp(float value, float min, float max) {
    if (value < min) value = min;
    if (value > max) value = max;

    return value;
}

float Math::Distance(Vector3 a, Vector3 b) {

    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z,2));
}


float Math::LerpEase(float oldVal, float val, float easingSpeed, EasingType easingType) {
    float t = 0.0f;
    switch (easingType) {
        case EasingType::EaseOutQuad: {
            t = 1.0f - pow(1.0f - easingSpeed, 2.0f);
            break;
        }
    }

    return Lerp(oldVal, val, t);
}

/*Example calls
LerpedYaw = Math::WrapDeg(LerpedYaw, Yaw - 180, Yaw + 180);
LerpedPitch = Math::Wrap(LerpedPitch, Pitch - 90, Pitch + 90);
 */
float Math::Wrap(float value, float min, float max) {
    return fmod(fmod(value - min, max - min) + (max - min), max - min) + min;
}

float Math::RandomFloat(float min, float max) {
    return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

bool Math::IsPosWithinAngle(float angle, Vector3 pos)
{
#ifndef __RELEASEFREE__
    // Floor the pos
    pos.x = floor(pos.x);
    pos.y = floor(pos.y);
    pos.z = floor(pos.z);

    // Add 0.5 to each axis
    pos.x += 0.5f;
    pos.y += 0.5f;
    pos.z += 0.5f;

    // Get the player's position
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return false;

    Vector3* playerPos = player->getPos();

    Vector2 rotations = Misc::GetRotations(playerPos, &pos);

    // Return true if the pitch AND yaw are within the angle range
    return (abs(rotations.x) <= angle && abs(rotations.y) <= angle);
#else
    return false;
#endif

}

Vector2 Math::CalculateMoveVector(Vector2 pitchYaw, Vector3 velocity)
{
    float pitchDeg = pitchYaw.x;
    float yawDeg = pitchYaw.y;

    float yawRad = ToRadians(yawDeg);

    // Calculate the direction vector based on the velocity relative to yaw
    Vector3 directionVector = Vector3(-sin(yawRad), 0.f, cos(yawRad));

    // Calculate the move vector based on the direction vector and the pitch
    Vector3 moveVector = directionVector * velocity.z;
    moveVector.y = sin(ToRadians(pitchDeg)) * velocity.z;
    moveVector.x = cos(yawRad) * cos(ToRadians(pitchDeg)) * velocity.z;

    return Vector2(moveVector.x, moveVector.z);
}