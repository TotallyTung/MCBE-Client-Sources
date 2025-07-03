#pragma once

class ToggleSounds : public Module
{
public:
    ToggleSounds(int keybind = Keys::NUM_0, bool enabled = false) :
        Module("ToggleSounds", "Misc", "The sound to play when a module is toggled", keybind, enabled)