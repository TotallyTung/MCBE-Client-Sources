#pragma once
class glide : public Module
{
public:
    glide(int keybind = Keys::NUM_0, bool enabled = false) :
        Module("glide", "Player", "glide", keybind, enabled)