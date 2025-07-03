
#pragma once
/*//
// Created by jcazm on 7/27/2024.
//
#include <Features/Modules/Module.hpp>

class ItemPhysics : public ModuleBase<ItemPhysics> {
    uint32_t origPosRel = 0;
    float* newPosRel = nullptr;
public:
    std::unordered_map<Actor*, std::tuple<float, glm::vec3, glm::ivec3>> actorData;
    struct ActorRenderData* renderData = nullptr;

    NumberSetting mSpeed = NumberSetting("Speed", "Rotation speed", 8.f, 1, 20, 0.01);
    NumberSetting mX = NumberSetting("X Multiplier", "The intensity of the x rotation", 18.f, 7.f, 30.f, 0.01f);
    NumberSetting mY = NumberSetting("Y Multiplier", "The intensity of the y rotation", 18.f, 7.f, 30.f, 0.01f);
    NumberSetting mZ = NumberSetting("Z Multiplier", "The intensity of the z rotation", 18.f, 7.f, 30.f, 0.01f);

    ItemPhysics() : ModuleBase("ItemPhysics", "Gives items physics", ModuleCategory::Visual, 0, false) {
        addSetting(&mSpeed);
        addSetting(&mX);
        addSetting(&mY);
        addSetting(&mZ);
        mNames = {
            {Lowercase, "itemphysics"},
            {LowercaseSpaced, "item physics"},
            {Normal, "ItemPhysics"},
            {NormalSpaced, "Item Physics"}
        };
    }
    static void glm_rotate(glm::mat4x4& mat, float angle, float x, float y, float z);
    void onEnable() override;
    void onDisable() override;
    void onRenderEvent(class RenderEvent& event);
    void onItemRendererEvent(class ItemRendererEvent& event);
};
*/
