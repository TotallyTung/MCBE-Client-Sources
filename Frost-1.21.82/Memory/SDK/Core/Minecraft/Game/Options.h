#pragma once

class IntOption {
public:
    char padding_16[16];     // this+0x00
    int mMaximum;            // this+0x10
    char padding_24[4];      // this+0x14
    int mValue;              // this+0x18
    char padding_32[4];      // this+0x1C
    int mMinimum;            // this+0x20
};
static_assert(sizeof(IntOption) == 36, "IntOption size is incorrect!");

class FloatOption
{
public:
    char pad_0000[16];      // this+0x00
    float mMinimum;          // this+0x10
    float mMaximum;          // this+0x14
    float mValue;            // this+0x18
    float mDefaultValue;    // this+0x1C
};

class Options
{
public:
    CLASS_FIELD(IntOption*, mThirdPerson, 0x30);
    CLASS_FIELD(void*, view_bob, 0x120);
    CLASS_FIELD(FloatOption*, mGfxFieldOfView, 0x1A0);
    CLASS_FIELD(FloatOption*, mGfxGamma, 0x1B8);

    FloatOption* getOption(int id) {
        int offset = 0x8 * id;
        return reinterpret_cast<FloatOption*>(reinterpret_cast<uintptr_t>(this) + offset);
    }
};