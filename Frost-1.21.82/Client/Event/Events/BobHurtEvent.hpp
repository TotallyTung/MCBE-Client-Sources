#pragma once

class BobHurtEvent : public Event
{
public:
    BobHurtEvent(glm::mat4* matrix)
    {
        mMatrix = matrix;
    }

    EventType getType() const override { return EventType::BobHurt; }
    glm::mat4* mMatrix;
};