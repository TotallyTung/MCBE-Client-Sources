//
// Created by vastrakai on 9/26/2024.
//

#include "MathLib.hpp"

#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Rendering/GuiData.hpp>

#include "lua.h"


void MathLib::initialize(lua_State* L)
{
    getGlobalNamespace(L)
    .beginClass<MathUtils>("MathUtils")
        .addStaticFunction("getRots", [](glm::vec3 pos, glm::vec3 target) -> glm::vec2 {
            return MathUtils::getRots(pos, target);
        })
        .addStaticFunction("getMotion", &MathUtils::getMotion)
        .addStaticFunction("getRotationKeyOffset", &MathUtils::getRotationKeyOffset)
        .addStaticFunction("distance", [](glm::vec3 a, glm::vec3 b) -> float {
            return glm::distance(a, b);
        })
    // getImBoxPoints
        .addStaticFunction("getImBoxPoints", [](AABB aabb) -> std::vector<ImVec2> {
            return MathUtils::getImBoxPoints(aabb);
        })
        .addStaticFunction("worldToScreen", [](glm::vec3 pos) -> glm::vec2 {
            ClientInstance* ci = ClientInstance::get();
            glmatrixf mat = ci->getViewMatrix();
            glm::vec2 screen;
            if (mat.OWorldToScreen(RenderUtils::transform.mOrigin, pos, screen, RenderUtils::transform.mFov, ci->getGuiData()->mResolution))
                return screen;

            return glm::vec2(-1, -1);
        })
        .addStaticFunction("wrapYaw", [](float yaw) -> float {
            return MathUtils::wrap(yaw, -180.f, 180.f);
        })
        .addStaticFunction("getAabbRect", [](AABB& aabb) -> std::vector<ImVec2> {
            ClientInstance* ci = ClientInstance::get();
            glmatrixf mat = RenderUtils::transform.mMatrix;
            auto result = mat.getRectForAABB(aabb, RenderUtils::transform.mOrigin, RenderUtils::transform.mFov, ci->getGuiData()->mResolution);
            std::vector<ImVec2> points;
            points.push_back(ImVec2(result.x, result.y));
            points.push_back(ImVec2(result.z, result.w));
            return points;
        }) // example: local rect = MathUtils.getRectForAABB(AABB.new(0, 0, 0, 1, 1, 1))
    .endClass()
    .beginClass<glm::vec2>("Vec2")
        .addConstructor<void(*)(float, float)>()
        .addProperty("x", &glm::vec2::x)
        .addProperty("y", &glm::vec2::y)
    .endClass()
    .beginClass<glm::vec3>("Vec3")
        .addConstructor<void(*)(float, float, float)>()
        .addProperty("x", &glm::vec3::x)
        .addProperty("y", &glm::vec3::y)
        .addProperty("z", &glm::vec3::z)
    .endClass()
    .beginClass<glm::ivec3>("Vec3i")
        .addConstructor<void(*)(int, int, int)>()
        .addProperty("x", &glm::ivec3::x)
        .addProperty("y", &glm::ivec3::y)
        .addProperty("z", &glm::ivec3::z)
    .endClass()
    .beginClass<glm::vec4>("ImVec4")
        .addConstructor<void(*)(float, float, float, float)>()
        .addProperty("x", &glm::vec4::x)
        .addProperty("y", &glm::vec4::y)
        .addProperty("z", &glm::vec4::z)
        .addProperty("w", &glm::vec4::w)
    .endClass()
    .beginClass<ImVec2>("ImVec2")
        .addConstructor<void(*)(float, float)>()
        .addProperty("x", &ImVec2::x)
        .addProperty("y", &ImVec2::y)
    .endClass()
    .beginClass<ImColor>("ImColor")
        .addConstructor<void(*)(int, int, int, int)>()
        .addProperty("Value", &ImColor::Value)
    .endClass()
    ;
    // getmotion lua example
    // local motion = MathUtils.getMotion(90, 1, false)
    // print(motion.x, motion.y)

}
