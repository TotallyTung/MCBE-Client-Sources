//
// Created by vastrakai on 10/10/2023.
//

#pragma once

#include <Prax/SDK/Game/Misc/Structs.h>
#include <Prax/Utils/System/Memory.h>

// size is 136
struct MoveInputComponent {
public:
    // Convert this into manually-padded struct
    BUILD_ACCESS(this, bool, moveLocked, 0x82);
    BUILD_ACCESS(this, bool, isSneakDown, 0x20);
    BUILD_ACCESS(this, bool, isJumping, 0x26);
    BUILD_ACCESS(this, bool, isJumping2, 0x80);
    BUILD_ACCESS(this, bool, isSprinting, 0x27);
    BUILD_ACCESS(this, bool, forward, 0x2A);
    BUILD_ACCESS(this, bool, backward, 0x2B);
    BUILD_ACCESS(this, bool, left, 0x2C);
    BUILD_ACCESS(this, bool, right, 0x2D);
    BUILD_ACCESS(this, Vector2, moveVector, 0x48);

    // padding to make the struct size 136
    char pad_0x0[0x88];

    void setJumping(bool value) {
        reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0x26)[0] = value;
        reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(this) + 0x80)[0] = value;
    }
};

static_assert(sizeof(MoveInputComponent) == 136, "MoveInputComponent size is not 136 bytes!");
