//
// Created by vastrakai on 10/21/2023.
//

#pragma once

class LookControlComponent {
public:
    char pad_0000[0x30]; //0x0000
};

// assert size to 48
static_assert(sizeof(LookControlComponent) == 0x30, "LookControlComponent is not the correct size. (0x30/48 bytes)");