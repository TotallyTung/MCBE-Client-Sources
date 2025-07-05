//
// Created by vastrakai on 10/10/2023.
//

#pragma once

#include <Prax/SDK/Game/Misc/Structs.h>
#include <Prax/Utils/System/Memory.h>

struct JumpControlComponent {
public:
    // padding to make the struct size 88 bytes
    BUILD_ACCESS(this, bool, noJumpDelay, 0x10);


    char pad_0x0[0x58];
};

static_assert(sizeof(JumpControlComponent) == 88, "JumpControlComponent size is not 88 bytes!");