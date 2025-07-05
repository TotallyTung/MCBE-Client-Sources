//
// Created by vastrakai on 1/10/2024.
//

#pragma once

struct MovementInterpolatorComponent {
    BUILD_ACCESS(this, Vector2, angles, 0xC0);
    // padding to make the size match
    char pad_0x0[0x28];
};

static_assert(sizeof(MovementInterpolatorComponent) == 0x28, "MovementInterpolatorComponent size is not 0x28 bytes!");

