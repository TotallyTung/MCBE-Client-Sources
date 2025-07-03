#pragma once

#include "../../../../Libraries/glm/glm/gtc/quaternion.hpp"

class CameraComponent {
public:
    HashedString mViewName{};
    glm::quat mQuat{};
    glm::vec3 mOrigin{};
    glm::vec4 mFov{}; // mViewport? (Fov X, Fov Y, Near, Far)?
    glm::mat4 mWorld{};
    glm::mat4 mView{};
    glm::mat4 mProjection{};
    int8_t padding[4]{ 0 };

    void copyFrom(const CameraComponent* other) {
        mQuat = other->mQuat;
        mOrigin = other->mOrigin;
        mFov = other->mFov;
        mWorld = other->mWorld;
        mView = other->mView;
        mProjection = other->mProjection;
    }
};
//static_assert(sizeof(CameraComponent) == 0x120);