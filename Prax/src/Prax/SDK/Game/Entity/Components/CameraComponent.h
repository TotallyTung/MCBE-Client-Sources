//
// Created by vastrakai on 1/10/2024.
//

#pragma once

struct CameraComponent {
    char pad_0x0[0x9C];
};

static_assert(sizeof(CameraComponent) == 0x9C, "CameraComponent size is not 156 bytes!");