#pragma once

class Velocity : public Module
{
public:
    int fulltype = 0;
    Velocity(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Velocity", "Combat", "anti knockback", keybind, enabled)
    {
        addEnum("Type", "type of velocity", { "Full" }, &fulltype);
    }
};