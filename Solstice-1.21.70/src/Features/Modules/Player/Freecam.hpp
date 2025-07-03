#pragma once
//
// Created by vastrakai on 7/22/2024.
//

#include <Features/Modules/Module.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/MinecraftSim.hpp>
#include <SDK/Minecraft/Actor/Components/ActorHeadRotationComponent.hpp>
#include <SDK/Minecraft/Actor/Components/ActorRotationComponent.hpp>
#include <SDK/Minecraft/Actor/Components/MobBodyRotationComponent.hpp>


class Freecam : public ModuleBase<Freecam> {
public:
    enum class Mode
    {
        Normal,
    };

    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode of the freecam\nNormal: Fakes the player's position\nDetached: Moves independently of the player", Mode::Normal, "Normal");
    NumberSetting mSpeed = NumberSetting("Speed", "Speed of the freecam", 5.5f, 0.1f, 10.0f, 0.1f);

    Freecam() : ModuleBase<Freecam>("Freecam", "Move independently of your player", ModuleCategory::Player, 0, false) {
        addSetting(&mMode);
        addSetting(&mSpeed);

        mNames = {
            {Lowercase, "freecam"},
            {LowercaseSpaced, "freecam"},
            {Normal, "Freecam"},
            {NormalSpaced, "Freecam"}
        };
    }

    ActorRotationComponent mLastRot;
    ActorHeadRotationComponent mLastHeadRot;
    MobBodyRotationComponent mLastBodyRot;
    AABB mOldAABB;
    glm::vec3 mAABBMin;
    glm::vec3 mAABBMax;
    glm::vec3 mSvPos;
    glm::vec3 mSvPosOld;
    glm::vec3 mOldPos;
    glm::vec3 mLastVelocity;

    // Detached mode vars
    glm::vec3 mOrigin;
    glm::vec3 mOldOrigin;
    glm::vec2 mRotRads;

    std::map<CameraMode, glm::vec2> mOriginalRotRads;
    std::unordered_map<EntityId, int> mCameras;
    bool mResetRot = false;
    glm::vec2 mHeadYaw;
    int mLastCameraState = 0;

    bool mShouldDisable = false;


    glm::vec3 getLerpedOrigin()
    {
        return MathUtils::lerp(mOldOrigin, mOrigin, ClientInstance::get()->getMinecraftSim()->getGameSim()->mDeltaTime);
    }

    void onEnable() override;
    void onDisable() override;
    void onPacketInEvent(class PacketInEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
    void onBaseTickEvent(class BaseTickEvent& event);
    void onRenderEvent(class RenderEvent& event);

    std::string getSettingDisplay() override
    {
        return mMode.mValues[mMode.as<int>()];
    }

};
