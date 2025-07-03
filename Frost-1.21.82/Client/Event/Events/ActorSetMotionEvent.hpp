#pragma once

class ActorSetMotionEvent : public Event
{
public:
    ActorSetMotionEvent(Vector3<float> motion)
    {
        mMotion = motion;
    }

    EventType getType() const override { return EventType::ActorSetMotion; }
    Vector3<float> mMotion;
};