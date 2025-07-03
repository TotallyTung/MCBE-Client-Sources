#pragma once

class CameraDirectLookComponent {
public:
    glm::vec2 mRotRads{};
    float mDelta = 0.0f;
    glm::vec2 mWrap{};
};