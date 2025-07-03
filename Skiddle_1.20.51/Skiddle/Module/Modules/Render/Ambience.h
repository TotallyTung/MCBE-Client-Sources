#pragma once

class Ambience : public Module
{
public:
    Ambience(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Ambience", "Render", "Change Time of Day", keybind, enabled)
    {
        registerEnumSetting("Mode", "The mode of ambience", { "Unchange", "Clear", "Night" }, &mode);
    }
    int mode = 0;

    void onEvent(GetTimeofDayEvent* event) override
    {
        switch (mode) {
        case 0: // unchange
            break;
        case 1: // clear
            *event->modifier = 1.f;
            break;
        case 2: // night
            *event->modifier = 0.5f;
            break;
        }
    }
};
