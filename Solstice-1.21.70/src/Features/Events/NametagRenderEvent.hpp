//
// Created by vastrakai on 11/10/2024.
//

#pragma once

#include "Event.hpp"

class NametagRenderEvent : public Event {
public:
    Actor* mActor;
    glm::vec3* mPos;
    bool mUnknown;
    float mDeltaTime;
    mce::Color* mColor;
    bool mCancelled = false;

    explicit NametagRenderEvent(Actor* actor, glm::vec3* pos, bool unknown, float deltaTime, mce::Color* color)
        : mActor(actor), mPos(pos), mUnknown(unknown), mDeltaTime(deltaTime), mColor(color) {}

    void cancel()
    {
        mPos->x = FLT_MAX;
        mPos->y = FLT_MAX;
        mPos->z = FLT_MAX;
        mCancelled = true;
    }
};