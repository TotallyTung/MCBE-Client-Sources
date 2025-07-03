#pragma once

#include "../../Utils/Math.h"

struct ActorRotationComponent {
public:
    Vec2<float> rotation;    
    Vec2<float> headRotation;
    Vec2<float> oldRotation;
    Vec2<float> oldHeadRotation;
    ActorRotationComponent(const Vec2<float>& rotation)
        : rotation(rotation), oldRotation(rotation),
        headRotation(rotation), oldHeadRotation(rotation) {
    }
    [[nodiscard]] const Vec2<float>& Get() const {
        return this->rotation;
    }
    void Set(const Vec2<float>& rots) {
        this->oldRotation = this->rotation;
        this->rotation = rots;
    }
    [[nodiscard]] float GetYaw() const {
        return this->rotation.y;
    }
    void SetYaw(float yaw) {
        this->oldRotation.y = this->rotation.y; 
        this->rotation.y = yaw;
    }
    [[nodiscard]] float GetPitch() const {
        return this->rotation.x;
    }
    void SetPitch(float pitch) {
        this->oldRotation.x = this->rotation.x;
        this->rotation.x = pitch;
    }
    [[nodiscard]] float GetHeadYaw() const {
        return this->headRotation.y;
    }
    void SetHeadYaw(float headYaw) {
        this->oldHeadRotation.y = this->headRotation.y;
        this->headRotation.y = headYaw;
    }
    [[nodiscard]] float GetHeadPitch() const {
        return this->headRotation.x;
    }
    void SetHeadPitch(float headPitch) {
        this->oldHeadRotation.x = this->headRotation.x;
        this->headRotation.x = headPitch;
    }
};
