#pragma once

#include "../Controls/RootUIControl.h"
#include "VisualTree.h"

class ScreenView
{
public:
    float             mLastTime;          // this+0x0000
    float             mDeltaTime;         // this+0x0004
    char              pad_0x0008[0x8];    // this+0x0008
    Vector2<float>    mScreenScale;       // this+0x0010
    char              pad_0x0018[0x20];   // this+0x0018
    ScreenController* mController;        // this+0x0038
    char              pad_0x0040[0x8];    // this+0x0040
    VisualTree*       mTree;              // this+0x0048

    template<typename Ret>
    Ret* getScreenController() {
        return reinterpret_cast<Ret*>(mController);
    }
};