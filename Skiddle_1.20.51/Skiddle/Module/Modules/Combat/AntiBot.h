#pragma once
//I need to sort this out soon lol

class AntiBot : public Module
{
public:
    AntiBot(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("AntiBot", "Combat", "No more bots :)", keybind, enabled)
    {}
};
