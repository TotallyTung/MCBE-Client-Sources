#pragma once

class DebugCameraComponent
{
public:
    HashedString mViewName{};
    glm::quat mQuat{};
    glm::vec3 mOrigin{};
    glm::vec4 mFov{}; // mViewport? (Fov X, Fov Y, Near, Far)?
    glm::mat4 mWorld{};
    glm::mat4 mView{};
    glm::mat4 mProjection{};
    int8_t padding[4]{ 0 };
};