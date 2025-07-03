#pragma once
//
// Created by vastrakai on 7/8/2024.
//

#include <vector>
#include <Utils/MemUtils.hpp>


class IntOption {
public:
    PAD(0x10);
    int maximum;
    PAD(0x4);
    int value;
    PAD(0x4);
    int minimum;
};

static_assert(sizeof(IntOption) == 36, "IntOption size is incorrect!");

class FloatOption
{
public:
    PAD(0x10);
    float mMinimum;
    float mMaximum;
    float mValue;
    float mDefaultValue;
};

class Options
{
public:
    CLASS_FIELD(IntOption*, mThirdPerson, 0x30);
    CLASS_FIELD(void*, mViewBob, 0x120);
    CLASS_FIELD(FloatOption*, mGfxFieldOfView, 0x1A0);
    CLASS_FIELD(FloatOption*, mGfxGamma, 0x1B8);
};
