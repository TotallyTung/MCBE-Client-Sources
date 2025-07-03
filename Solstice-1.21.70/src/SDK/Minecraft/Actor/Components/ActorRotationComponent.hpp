//
// Created by vastrakai on 6/30/2024.
//

#pragma once

struct ActorRotationComponent {
public:
    float mPitch;
    float mYaw;
    float mOldPitch;
    float mOldYaw;
};

static_assert(sizeof(ActorRotationComponent) == 0x10);