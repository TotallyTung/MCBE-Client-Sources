#pragma once

#include "Game/NetEventCallback.h"
#include "Game/GameSession.h"

class Simulation {
public:
    float       mTimer = 20.f;             // this+0x0
    char        pad_0x4[0x4];              // this+0x4
    float       mDeltaTime = 0.f;          // this+0x8
    float       mTimerMultiplier = 1.f;    // this+0xC
    float       mLastDeltaTime = 0.f;      // this+0x10
    char        pad_0x14[0x4];             // this+0x14
    float       mTotalTime = 1.f;          // this+0x18
    float       mFrameDelta;               // this+0x1C
};

// TimerClass known as Minecraft class
class Minecraft
{
public:
    char                         pad_0x0[0xD0];     // this+0x0
    GameSession*                 mGameSession;       // this+0xC8
    char                         pad_0xC8[0x10];     // this+0xD0
    Simulation*                  mGameSimulation;    // this+0xE0
    Simulation*                  mRenderSimulation;  // this+0xE8

public:
    void setTimer(float timer) {
        this->mGameSimulation->mTimer = timer;
    }

    void setRenderTimer(float timer) {
        this->mRenderSimulation->mTimer = timer;
    }

    void setMainTimer(float timer) {
        setTimer(timer);
        setRenderTimer(timer);
    }

    void setSpeed(float speed) {
        this->mGameSimulation->mTimerMultiplier = speed;
    }

    void setRenderSpeed(float speed) {
        this->mRenderSimulation->mTimerMultiplier = speed;
    }

    void setMainSpeed(float speed) {
        setSpeed(speed);
        setRenderSpeed(speed);
    }
};