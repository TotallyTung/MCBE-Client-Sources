#pragma once

class BlockComponentStorage {
public:
    char pad_0000[0x38];  // Padding for alignment
    char mUnkbd91b3[56];  // this + 0x38
    char mUnkce79bc[40];  // this + 0x70
    bool mUnk3ed799;      // this + 0x98
    bool mUnkfe00f1;      // this + 0x99
    bool mUnkee4bb9;      // this + 0x9A
};