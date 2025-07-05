//
// Created by Flash on 10/15/2023.
//

#pragma once

#include "src/Prax/SDK/Game/World/Block.h"
#include "src/Prax/Utils/System/Internet.h"
#include "src/Prax/Utils/System/Memory.h"
#include "src/Prax/SDK/Game/Network/Network.h"
#include <Prax/SDK/Game/Misc/Structs.h>

class IntOption {
public:
    /* Fields */
    char padding_16[16];
    int maximum;
    char padding_24[4];
    int value;
    char padding_32[4];
    int minimum;
    /* Virtuals */
    /* Functions */


    /*const int VALUE_MAX;
    const int VALUE_MIN;
    int mValue;
    int mDefaultValue;
    bool mClampToRange;
    std::vector<int,std::allocator<int> > mPossibleValues;
    std::function<int __cdecl(int)> mCoerceSaveValueCallback;*/
};

class FloatOption
{
public:
    char pad_0000[16]; //0x0000
    float minimum;
    float maximum;
    float value;
    float default_value;
}; //Size: 0x0020

class Options
{
public:
    BUILD_ACCESS(this, IntOption*, game_thirdperson, 0x30);
    BUILD_ACCESS(this, void*, view_bob, 0x120);
    BUILD_ACCESS(this, FloatOption*, gfx_field_of_view, 0x1A0);
    BUILD_ACCESS(this, FloatOption*, gfx_gamma, 0x1B8);

    FloatOption* getOption(int id) {
        // Convert id to offset
        int offset = 0x8 * id;
        return reinterpret_cast<FloatOption*>(reinterpret_cast<uintptr_t>(this) + offset);
    }
};
