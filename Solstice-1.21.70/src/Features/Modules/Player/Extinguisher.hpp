//
// Created by alteik on 21/09/2024.
//
#pragma once
#include <Features/Modules/Module.hpp>

class Extinguisher : public ModuleBase<Extinguisher> {
public:

    NumberSetting mRange = NumberSetting("Range", "Range of the extinguisher", 7, 0, 10, 0.01);
    BoolSetting mRotate = BoolSetting("Rotate", "Enable or disable rotation", true);

    Extinguisher() : ModuleBase("Extinguisher", "Destroys fire", ModuleCategory::Player, 0, false) {

        addSetting(&mRange);
        addSetting(&mRotate);

        mNames = {
                {Lowercase, "extinguisher"},
                {LowercaseSpaced, "extinguisher"},
                {Normal, "Extinguisher"},
                {NormalSpaced, "Extinguisher"},
        };
    }

    bool mShouldRotate = false;
    glm::vec3 mCurrentPosition = {0.f, 0.f, 0.f};

    bool isValidBlock(glm::ivec3 blockPos);
    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
};