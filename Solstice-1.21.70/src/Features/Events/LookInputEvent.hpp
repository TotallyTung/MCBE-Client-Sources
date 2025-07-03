//
// Created by vastrakai on 7/22/2024.
//

#pragma once

#include <SDK/Minecraft/Actor/EntityContext.hpp>
#include <SDK/Minecraft/Actor/Components/CameraComponent.hpp>

class LookInputEvent : public CancelableEvent {
public:
    EntityContext* mEntityContext{};
    CameraComponent* mFirstPersonCamera{};
    CameraComponent* mThirdPersonCamera{};
    CameraComponent* mThirdPersonFrontCamera{};
    CameraComponent* mDeathCamera{};
    CameraComponent* mFreeCamera{};
    CameraDirectLookComponent* mCameraDirectLookComponent{};
    glm::vec2 mVec2{};

    explicit LookInputEvent(EntityContext* entityContext, CameraComponent* firstPersonCamera, CameraComponent* thirdPersonCamera, CameraComponent* thirdPersonFrontCamera, CameraComponent* deathCamera, CameraComponent* freeCamera, CameraDirectLookComponent* cameraDirectLookComponent, const glm::vec2& vec2)
        : mEntityContext(entityContext), mFirstPersonCamera(firstPersonCamera), mThirdPersonCamera(thirdPersonCamera), mThirdPersonFrontCamera(thirdPersonFrontCamera), mDeathCamera(deathCamera), mFreeCamera(freeCamera), mCameraDirectLookComponent(cameraDirectLookComponent), mVec2(vec2) {}
};