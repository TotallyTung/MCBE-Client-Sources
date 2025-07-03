#pragma once
#include "../../../Utils/MemoryUtil.h"
#include "../../Core/mce.h"
#include <vector>

class MobEffectInstance {
public:
	uint32_t mId;
    int mDuration;                        // this+0x4
    int mDurationEasy;                    // this+0x8
    int mDurationNormal;                  // this+0xC
    int mDurationHard;                    // this+0x10
    int mAmplifier;                       // this+0x14
    bool mDisplayOnScreenTextureAnimation; // this+0x18
    bool mAmbient;                         // this+0x19
    bool mNoCounter;                       // this+0x1A
    bool mEffectVisible;                   // this+0x1B
public:
    std::string getDisplayName() {
        // Search "potion.potency." and compare to server database!
        using func_t = std::string*(__thiscall*)(MobEffectInstance*, std::string&);
        static func_t Func = (func_t)Addresses::MobEffectInstance_getDisplayName;
        std::string ncct;
        return *Func(this, ncct);
    }
};
