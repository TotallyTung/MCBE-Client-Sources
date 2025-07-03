//
// Created by vastrakai on 7/25/2024.
//

#pragma once

#include "Event.hpp"

class BoneRenderEvent : public Event
{
public:
    class Bone* mBone;
    class ActorPartModel* mPartModel;
    class Actor* mActor;

    // only used internally if mActor == localPlayer
    bool mDoBlockAnimation = false;

    explicit BoneRenderEvent(Bone* bone, ActorPartModel* partModel, Actor* actor) : mBone(bone), mPartModel(partModel), mActor(actor) {}
};