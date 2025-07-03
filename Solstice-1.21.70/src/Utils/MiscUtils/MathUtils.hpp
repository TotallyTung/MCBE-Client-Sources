#pragma once
//
// Created by vastrakai on 6/29/2024.
//

#include <atomic>
#include <cmath>
#include <vector>


#include <Utils/Structs.hpp>

struct SPolygon {
    std::vector<ImVec2> points;
    ImColor color;

    [[nodiscard]] bool overlaps(const SPolygon& other) const {
        for (const auto& point : points) {
            if (other.contains(point)) return true;
        }
        return false;
    }

    [[nodiscard]] bool contains(const ImVec2& point) const {
        bool result = false;
        for (int i = 0, j = points.size() - 1; i < points.size(); j = i++) {
            if ((points[i].y > point.y) != (points[j].y > point.y) &&
                (point.x < (points[j].x - points[i].x) * (point.y - points[i].y) / (points[j].y - points[i].y) + points[i].x)) {
                result = !result;
            }
        }
        return result;
    }
};

class MathUtils {
public:
    static inline glm::vec2 fov = { 0, 0 };
    static inline glm::vec2 displaySize = { 0, 0 };
    static inline glm::vec3 origin = { 0, 0, 0 };

    static float animate(float endPoint, float current, float speed);
    static float lerp(float a, float b, float t);
    static glm::vec3 lerp(glm::vec3& a, glm::vec3& b, float t);
    static ImVec4 lerp(ImVec4& a, ImVec4& b, float t);
    static ImVec2 lerp(ImVec2& a, ImVec2& b, float t);
    static ImColor lerpImColor(ImColor& a, ImColor& b, float t);
    static float getRotationKeyOffset(bool allowStrafe = true);
    static glm::vec2 getMotion(float yaw, float speed, bool allowStrafe = true);
    template <typename T>
    static T clamp(T value, T min, T max) {
        return std::max(min, std::min(value, max));
    }
    static float clamp(float value, float min, float max) {
        return std::max(min, std::min(value, max));
    }

    static float random(float min, float max);
    static int random(int min, int max);
    template <typename T>
    static T random(T min, T max) {
        return random(static_cast<T>(min), static_cast<T>(max));
    }

    static float wrap(float val, float min, float max);
    static std::vector<glm::vec2> getBoxPoints(const AABB& aabb);
    static std::vector<ImVec2> getImBoxPoints(const AABB& aabb);

    static glm::vec2 getRots(const glm::vec3& pEyePos, const glm::vec3& pTarget);
    static glm::vec2 getRots(const glm::vec3& pEyePos, const AABB& target);
    static float snapYaw(float yaw);
    static glm::vec2 getMovement();
    static bool rayIntersectsAABB(glm::vec3 rayPos, glm::vec3 rayEnd, glm::vec3 hitboxMin, glm::vec3 hitboxMax);
    static float randomFloat(float min, float max);
};
