//
// Created by Flash on 10/15/2023.
//

#pragma once

#include "src/Prax/SDK/Game/World/Block.h"

class Timings {
public:
    float timer; // 0x0
    int idk; // 0x4
    float deltaTime; // 0x8
    float timerMultiplier; // 0xC
    float deltaTime2; // 0x10
    float idk2; // 0x14
    float totalTime; // 0x18
};

class GameTimings {
private:
    char padding[0xD8];
public:
    Timings* timer; // 0xD8
    Timings* renderTimer; // 0xE0
};
