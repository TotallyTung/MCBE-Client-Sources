#pragma once

class Velocity : public Module
{
public:
    Velocity() :
        Module("Velocity", "Motion", "Stops you from recieving knockback.") {}

    void onEvent(ActorSetMotionEvent* event) override {
        *event->mCancelled = true;
    }

    std::string getModeName() override {
        return std::string("Full");
    }
};