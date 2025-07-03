#pragma once

class Ambience : public Module
{
public:
    Ambience(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Ambience", "Visual", "Changes your day time", keybind, enabled)
    {
        addEnum("Day Time", "Day time enum", { "UnChanged", "Night", "Day", "Value" }, &Game::Core::TimeMode);
        addSlider("DayTime", "Day time value (only for value mode)", &Game::Core::DayTimeValue, 0.f, 1);
    }
};
