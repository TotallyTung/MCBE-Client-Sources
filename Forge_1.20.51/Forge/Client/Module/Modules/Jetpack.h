#pragma once

class Jetpack : public Module
{
public:
    Jetpack(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Jetpack", "Movement", "Flys to where your crosshair is located", keybind, enabled)
    {

    }

    void onEvent(UpdateEvent* event) override {
        float calcYaw = static_cast<float>((Game::GetInstance()->getLocalPlayer()->getRotation()->y + 90) * (3.14159265359 / 180));
        float calcPitch = static_cast<float>((Game::GetInstance()->getLocalPlayer()->getRotation()->x) * -(3.14159265359 / 180));

        Vector3<float> moveVec;
        moveVec.x = cos(calcYaw) * cos(calcPitch) * 1.5;
        moveVec.y = sin(calcPitch) * 1.5;
        moveVec.z = sin(calcYaw) * cos(calcPitch) * 1.5;

        Game::GetInstance()->getLocalPlayer()->lerpMotion(moveVec);
    }
};