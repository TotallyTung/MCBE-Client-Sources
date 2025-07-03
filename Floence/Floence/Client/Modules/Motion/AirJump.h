#pragma once

class AirJump : public Module
{
public:
    AirJump(int keybind = 7, bool enabled = false) :
        Module("Airjump", "Motion", "Jumps even if you don't touch the ground.", keybind, enabled)
    {
        addEnum("Rotations", "The mode of airjump", { "Legacy" }, &mode);
    }

    int mode = 0;

    void onEvent(RenderContextEvent* event) override {
        if (!Address::getLocalPlayer())
            return;

        Address::getLocalPlayer()->setIsOnGround(true);
    }

    void onDisabled() override {
        if (!Address::getLocalPlayer())
            return;

        Address::getLocalPlayer()->setIsOnGround(false);
    }
};