#pragma once

struct SyncedAttribute {
public:
    std::string mName;         // this+0x0
    float       mMinValue;     // this+0x20
    float       mCurrentValue; // this+0x24
    float       mMaxValue;     // this+0x28
};