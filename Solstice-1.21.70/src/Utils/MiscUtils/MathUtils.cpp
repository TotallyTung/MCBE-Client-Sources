//
// Created by vastrakai on 6/29/2024.
//


#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Actor/Components/MoveInputComponent.hpp>
#include <Utils/Keyboard.hpp>
#include "RenderUtils.hpp"
#include "MathUtils.hpp"

#include <random>
#include <SDK/Minecraft/KeyboardMouseSettings.hpp>


float MathUtils::animate(float endPoint, float current, float speed) { // Animate the position
    if (speed < 0.0) speed = 0.0; // If 0 is less than speed then set speed to 0.
    else if (speed > 1.0) speed = 1.0; // If Speed is faster than 1 then set speed to 1.

    float dif = std::fmax(endPoint, current) - std::fmin(endPoint, current); // Animate between max and min.
    float factor = dif * speed;
    return current + (endPoint > current ? factor : -factor); // Animates the distance
}

float MathUtils::lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

glm::vec3 MathUtils::lerp(glm::vec3& a, glm::vec3& b, float t)
{
    return a + t * (b - a);
}

ImVec4 MathUtils::lerp(ImVec4& a, ImVec4& b, float t)
{
    return ImVec4(lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t), lerp(a.w, b.w, t));
}

ImVec2 MathUtils::lerp(ImVec2& a, ImVec2& b, float t)
{
    return ImVec2(lerp(a.x, b.x, t), lerp(a.y, b.y, t));
}

ImColor MathUtils::lerpImColor(ImColor& a, ImColor& b, float t)
{
    return ImColor(lerp(a.Value.x, b.Value.x, t), lerp(a.Value.y, b.Value.y, t), lerp(a.Value.z, b.Value.z, t), lerp(a.Value.w, b.Value.w, t));
}

float MathUtils::getRotationKeyOffset(bool allowStrafe)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return 0;
    auto moveInput = player->getMoveInputComponent();

    bool isMoving = moveInput->mForward || moveInput->mBackward || moveInput->mLeft || moveInput->mRight;
    if (!isMoving) return 0;

    bool w = moveInput->mForward;
    bool s = moveInput->mBackward;
    bool a = moveInput->mLeft;
    bool d = moveInput->mRight;

    float yawOffset = 0;
    if (w && a && allowStrafe)
        yawOffset = -45;
    else if (w && d && allowStrafe)
        yawOffset = 45;
    else if (s && a && allowStrafe)
        yawOffset = -135;
    else if (s && d && allowStrafe)
        yawOffset = 135;
    else if (w)
        yawOffset = 0;
    else if (a && allowStrafe)
        yawOffset = -90;
    else if (s)
        yawOffset = -180;
    else if (d && allowStrafe)
        yawOffset = 90;
    else
        yawOffset = 0;

    return yawOffset;
}


glm::vec2 MathUtils::getMotion(float yaw, float speed, bool allowStrafe) {
    yaw += getRotationKeyOffset(allowStrafe) + 90;

    float calcYaw = glm::radians(yaw);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return { 0, 0 };

    if (!Keyboard::isUsingMoveKeys()) return { 0, 0 };

    glm::vec2 motion;
    motion.x = cos(calcYaw) * (speed);
    motion.y = sin(calcYaw) * (speed);

    return motion;
}

float MathUtils::wrap(float val, float min, float max)
{
    return fmod(fmod(val - min, max - min) + (max - min), max - min) + min;
}

float MathUtils::random(float min, float max)
{
    return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

int MathUtils::random(int min, int max)
{
    // Use random_device to get a random seed
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(min, max);
    return distr(eng);
}

#define PI IM_PI

std::vector<glm::vec2> MathUtils::getBoxPoints(const AABB& aabb) {
    ClientInstance* ci = ClientInstance::get();
    if (!ci->getLocalPlayer()) return {};

    auto& corrected = RenderUtils::transform.mMatrix;

    glm::vec3 worldPoints[8] = {
        {aabb.mMin.x, aabb.mMin.y, aabb.mMin.z},
        {aabb.mMin.x, aabb.mMin.y, aabb.mMax.z},
        {aabb.mMax.x, aabb.mMin.y, aabb.mMin.z},
        {aabb.mMax.x, aabb.mMin.y, aabb.mMax.z},
        {aabb.mMin.x, aabb.mMax.y, aabb.mMin.z},
        {aabb.mMin.x, aabb.mMax.y, aabb.mMax.z},
        {aabb.mMax.x, aabb.mMax.y, aabb.mMin.z},
        {aabb.mMax.x, aabb.mMax.y, aabb.mMax.z}
    };

    std::vector<glm::vec2> points;
    points.reserve(8);
    for (const auto& wp : worldPoints) {
        glm::vec2 result = {0, 0};
        if (!corrected.OWorldToScreen(origin, wp, result, fov, displaySize)) return {};
        if (result != glm::vec2(FLT_MAX, FLT_MAX)) {
            points.push_back(result);
        } else {
            return {};
        }
    }

    if (points.size() < 3) return {};

    auto it = std::ranges::min_element(points, [](const glm::vec2& a, const glm::vec2& b) {
        return a.x < b.x;
    });
    glm::vec2 start = *it;

    std::vector<glm::vec2> indices;
    indices.reserve(8);
    indices.push_back(start);

    glm::vec2 current = start;
    glm::vec2 lastDir = glm::vec2(0, -1);

    do {
        float smallestAngle = 2 * PI;
        glm::vec2 smallestDir = {0, 0};
        glm::vec2 smallestE = {0, 0};
        float lastDirAtan2 = atan2(lastDir.y, lastDir.x);

        for (const auto& t : points) {
            if (current == t) continue;

            glm::vec2 dir = t - current;
            float angle = atan2(dir.y, dir.x) - lastDirAtan2;
            if (angle > PI) angle -= 2 * PI;
            else if (angle <= -PI) angle += 2 * PI;

            if (angle >= 0 && angle < smallestAngle) {
                smallestAngle = angle;
                smallestDir = dir;
                smallestE = t;
            }
        }

        indices.push_back(smallestE);
        lastDir = smallestDir;
        current = smallestE;

    } while (current != start && indices.size() < 8);

    return indices;
}

std::vector<ImVec2> MathUtils::getImBoxPoints(const AABB& aabb) {
    std::vector<glm::vec2> points = getBoxPoints(aabb);
    std::vector<ImVec2> imPoints = {};
    for (auto point : points)
    {
        imPoints.emplace_back(point.x, point.y);
    }
    return imPoints;
}

glm::vec2 MathUtils::getRots(const glm::vec3& pEyePos, const glm::vec3& pTarget)
{
    glm::vec3 delta = pTarget - pEyePos;
    // -sin and cos for pitch and yaw respectively
    // Generate with rads first
    float yaw = atan2(delta.z, delta.x);
    yaw = glm::degrees(yaw) - 90;
    yaw = wrap(yaw, -180, 180);
    const float pitch = atan2(delta.y, sqrt(delta.x * delta.x + delta.z * delta.z)) * 180.0f / IM_PI;

    return {-pitch, yaw};
}

glm::vec2 MathUtils::getRots(const glm::vec3& pEyePos, const AABB& target)
{
    return getRots(pEyePos, target.getClosestPoint(pEyePos));
}

float MathUtils::snapYaw(float yaw)
{// Snap to a cardinal direction (range: -180 to 180)
    if (yaw < -135 || yaw > 135) return -180;
    if (yaw < -45) return -90;
    if (yaw < 45) return 0;
    if (yaw < 135) return 90;
    return 180;
}

float MathUtils::randomFloat(float min, float max)
{
    // Use random_device to get a random seed
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_real_distribution<float> distr(min, max);
    return distr(eng);
}

glm::vec2 MathUtils::getMovement() {
    glm::vec2 ret = glm::vec2(0, 0);
    float forward = 0.0f;
    float side = 0.0f;
    auto& keyboard = *ClientInstance::get()->getKeyboardSettings();

    bool w = Keyboard::mPressedKeys[keyboard["key.forward"]];
    bool a = Keyboard::mPressedKeys[keyboard["key.left"]];
    bool s = Keyboard::mPressedKeys[keyboard["key.back"]];
    bool d = Keyboard::mPressedKeys[keyboard["key.right"]];
    bool space = Keyboard::mPressedKeys[keyboard["key.jump"]];
    bool shift = Keyboard::mPressedKeys[keyboard["key.sneak"]];

    if (!w && !a && !s && !d)
        return ret;

    static constexpr float forwardF = 1;
    static constexpr float sideF = 0.7071067691f;

    if (w) {
        if (!a && !d)
            forward = forwardF;
        if (a) {
            forward = sideF;
            side = sideF;
        }
        else if (d) {
            forward = sideF;
            side = -sideF;
        }
    }
    else if (s) {
        if (!a && !d)
            forward = -forwardF;
        if (a) {
            forward = -sideF;
            side = sideF;
        }
        else if (d) {
            forward = -sideF;
            side = -sideF;
        }
    }
    else if (!w && !s) {
        if (!a && d) side = -forwardF;
        else side = forwardF;
    }

    ret.x = side;
    ret.y = forward;
    return ret;
}

bool MathUtils::rayIntersectsAABB(glm::vec3 rayPos, glm::vec3 rayEnd, glm::vec3 hitboxMin, glm::vec3 hitboxMax)
{
    glm::vec3 t0 = (hitboxMin - rayPos) / (rayEnd - rayPos);
    glm::vec3 t1 = (hitboxMax - rayPos) / (rayEnd - rayPos);

    glm::vec3 tmin = glm::min(t0, t1);
    glm::vec3 tmax = glm::max(t0, t1);

    float tminmax = glm::max(tmin.x, glm::max(tmin.y, tmin.z));
    float tmaxmin = glm::min(tmax.x, glm::min(tmax.y, tmax.z));

    return tminmax <= tmaxmin;
}
