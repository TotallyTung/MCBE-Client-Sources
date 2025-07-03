#pragma once

#include "MobEffect.h"

class MobEffectInstance {
public:
    // prevent constructor by default
    MobEffectInstance();

    MobEffect::EffectType            mId;                              // this+0x0
    int                              mDuration;                        // this+0x4
    int                              mDurationPeaceful;                // this+0x8
    int                              mDurationEasy;                    // this+0xC
    int                              mDurationNormal;                  // this+0x10
    int                              mDurationHard;                    // this+0x14
    int                              mAmplifier;                       // this+0x18
    char                             pad_0008[0x8];                    // this+0x1C
    bool                             mDisplayOnScreenTextureAnimation; // this+0x24
    bool                             mAmbient;                         // this+0x25
    bool                             mNoCounter;                       // this+0x26
    bool                             mEffectVisible;                   // this+0x27
    MobEffect::FactorCalculationData mFactorCalculationData;           // this+0x28 
};
static_assert(sizeof(MobEffectInstance) == 0x88);