#pragma once

class Animations : public Module
{
public:
    Animations(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Animations", "Visual", "Change how you swing", keybind, enabled)
    {
        addSlider("Swing Speed", "Change your swing speed", &Game::Core::SwingSpeed, 1, 70);

    }
};