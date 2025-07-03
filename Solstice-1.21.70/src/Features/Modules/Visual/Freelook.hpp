#pragma once
//
// Created by vastrakai on 11/2/2024.
//

#include <SDK/Minecraft/Actor/EntityId.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Options.hpp>
#include <SDK/Minecraft/Actor/Components/CameraComponent.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <Features/Events/LookInputEvent.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>

class Freelook : public ModuleBase<Freelook> {
public:
    Freelook() : ModuleBase<Freelook>("Freelook", "Allows you to freelook", ModuleCategory::Visual, 0, false) {
        mNames = {
            {Lowercase, "freelook"},
            {LowercaseSpaced, "free look"},
            {Normal, "Freelook"},
            {NormalSpaced, "Free Look"}
        };

        mEnableWhileHeld = true;

        gFeatureManager->mDispatcher->listen<LookInputEvent, &Freelook::onLookInputEvent>(this);
    }

    int mLastCameraState = 0;
    glm::vec2 mLookingAngles = glm::vec2(0.0f, 0.0f);
    bool mHadComponent = false;
    std::map<CameraMode, glm::vec2> mOriginalRotRads;
    std::unordered_map<EntityId, int> mCameras;
    bool mResetRot = false;
    glm::vec2 mHeadYaw;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(BaseTickEvent& event);
    void onPacketOutEvent(PacketOutEvent& event);
    void onLookInputEvent(LookInputEvent& event);
};