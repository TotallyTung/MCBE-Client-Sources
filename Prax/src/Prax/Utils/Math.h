//
// Created by Flash on 7/11/2023.
//

#pragma once

#include <imgui.h>
#include <Prax/SDK/Game/Misc/Structs.h>

enum class EasingType {
    EaseOutQuad,
    EaseOutCubic,
    EaseOutQuart,
    EaseOutQuint,
    EaseOutSine,
    EaseOutExpo,
    EaseOutCirc,
};

class Math {
public:

    static Vector2 Fov;
    static ImColor Color;
    static Vector3 Origin;
    static Vector2 ScreenSize;
    static ImDrawList* DrawList;

    // Use drawlist to draw a 3d box around a center position
    static float EaseInOutExpo(float pct);
    static bool DrawBox(Vector3 lower, Vector3 upper, float lineWidth);
    static bool DrawBox(Vector3 lower, Vector3 upper, float lineWidth, ImColor color);
    static bool DrawBoxFilled(Vector3 lower, Vector3 upper, float lineWidth, ImColor borderColor, ImColor filledColor, bool renderEdges = true);
    static void DrawQuad(ImVec2 p1, ImVec2 p2, ImVec2 p3, ImVec2 p4, float lineWidth = 1.0f);
    static void DrawQuad(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, float lineWidth = 1.0f);
    static void DrawLine(Vector2 start, Vector2 end, float lineWidth = 1.0f);
    static ImVec2 Lerp(ImVec2 a, ImVec2 b, float t);
    static float Lerp(float a, float b, float c);
    static Vector3 Lerp(Vector3 a, Vector3 b, float t);
    static float Animate(float current, float endPoint, float speed);
    static float ToRadians(float deg);
    static int RandomInt(int min, int max);
    static float Clamp(float value, float min, float max);
    static float Distance(Vector3 a, Vector3 b);
    static float LerpEase(float oldVal, float val, float easingSpeed, EasingType easingType);
    static float Wrap(float yaw, float d, float d1);
    static float RandomFloat(float min, float max);
    static Vector2 CalculateMoveVector(Vector2 pitchYaw, Vector3 velocity);
    static bool IsPosWithinAngle(float angle, Vector3 pos);
    static ImVec4 Get2dDrawPoints(Vector3 lower, Vector3 upper);
    static std::vector<Vector2> GetHexForAABB(AABB aabb);
    static bool DrawOutlinedBox(Vector3 lower, Vector3 upper, float x, ImColor color);
    static bool DrawOutlinedBoxNaCl(Vector3 lower, Vector3 upper, float x, ImColor color);
    static float GetDistance(ImVec2 a, ImVec2 b);
    static float GetDistanceBetweenPoints(Vector2 value, ImVec2 center);
    static Vector2 GetClosestPointOnLine(const std::vector<Vector2>& vector, ImVec2 center);
    static bool LineIntersection(ImVec2 start, ImVec2 end, ImVec2 start2, ImVec2 end2, ImVec2& point);
    static float DegToRad(float x);
    static float Round(float bps, int decimals);
    static float SnapYaw(float x);
    static bool DrawFilledOutlinedBox(Vector3 lower, Vector3 upper, float x, ImColor color, ImColor outlineColor);
};