#pragma once

class ViewModel : public Module {
public:
    ViewModel(int keybind = Keys::NONE, bool enabled = false) :
        Module("ViewModel", "Visual", "Move your item's position", keybind, enabled) {
        addSlider("PosX", "Changes bob hurt position", &viewHurtItemX, -300, 300);
        addSlider("PosY", "Changes bob hurt position", &viewHurtItemY, -300, 300);
        addSlider("PosZ", "Changes bob hurt position", &viewHurtItemZ, -300, 300);

        addSlider("Rot Angle", "Changes bob hurt rotation angle", &viewHurtItemRotAngle, 0, 360);

        addSlider("RotX", "Changes bob hurt rotation", &viewHurtItemRotX, 0, 360);
        addSlider("RotY", "Changes bob hurt rotation", &viewHurtItemRotY, 0, 360);
        addSlider("RotZ", "Changes bob hurt rotation", &viewHurtItemRotZ, 0, 360);
        addBool("Block only", "Chicken wing", &blockOnly);
    }

    float viewHurtItemX = 0.3f;
    float viewHurtItemY = -0.1f;
    float viewHurtItemZ = -1.0f;

    float viewHurtItemRotAngle = 0.0f;
    float viewHurtItemRotX = 0.0f;
    float viewHurtItemRotY = 0.0f;
    float viewHurtItemRotZ = 0.0f;
    bool blockOnly = false;

    void onEvent(ViewBobbingTickEvent* event) override {
            glm::mat4& matrix = *event->Matrix;

            matrix = glm::translate(matrix, glm::vec3(viewHurtItemX / 100, viewHurtItemY / 100, viewHurtItemZ / 100));

            if (viewHurtItemRotAngle != 0.0f || viewHurtItemRotX != 0.0f || viewHurtItemRotY != 0.0f || viewHurtItemRotZ != 0.0f) {
                matrix = glm::rotate(matrix, glm::radians(viewHurtItemRotAngle), glm::vec3(viewHurtItemRotX, viewHurtItemRotY, viewHurtItemRotZ));
            }
        }
};
