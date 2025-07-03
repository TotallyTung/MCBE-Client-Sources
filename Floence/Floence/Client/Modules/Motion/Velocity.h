#pragma once

class Velocity : public Module
{
public:
    Velocity(int keybind = 7, bool enabled = true) :
        Module("Velocity", "Motion", "Increase your game tick rate.", keybind, enabled)
    {
        addEnum("Style", "Da mode of velocity!", { "Full", "Adaptive", "Test" }, &Global::VelocityMode);
    }
    int Style = 0;

//    std::string getModeName() override {
//        return " " + std::string("Full");
//    }
};