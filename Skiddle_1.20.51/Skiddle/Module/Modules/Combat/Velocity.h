#pragma once

class Velocity : public Module
{
public:
    Velocity(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Velocity", "Combat", "Change your knockback", keybind, enabled)
    {
        registerFloatSetting("Vertical", "Modify your Vertical KB", &y, 0, 10);
        registerFloatSetting("Horizontal", "Modify your Horizontal KB", &x, 0, 10);
    }

    float x = 0;
    float y = 0;

    void onEvent(LerpMotionEvent* event) override
    {
        Vector3<float>* motion = event->Motion;
        StateVectorComponent* stateVector = event->Actor->try_get<StateVectorComponent>();

        if (!stateVector) {
            return;
        }

        motion->x *= x / 10;
        motion->y *= y / 10;
        motion->z *= x / 10;

        *motion = stateVector->r_Velocity.lerp(*motion, x, y, x);
    }


    std::string getModeName() override
    {
        std::ostringstream oss;
        oss.precision(0);
        oss << " " << std::fixed << x << "% " << std::fixed << y << "%";
        return oss.str();
    }
};