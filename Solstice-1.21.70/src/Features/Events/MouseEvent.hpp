//
// Created by vastrakai on 6/29/2024.
//

#pragma once
#include "Event.hpp"

class MouseEvent : public CancelableEvent {
    // void* _this, char actionButtonId, char buttonData, __int16 x, __int16 y, __int16 dx, __int16 dy, bool forceMotionlessPointer);
public:
    void* mThis;
    char mActionButtonId;
    char mButtonData;
    __int16 mX;
    __int16 mY;
    __int16 mDx;
    __int16 mDy;
    bool mForceMotionlessPointer;

    explicit MouseEvent(void* _this, char actionButtonId, char buttonData, __int16 x, __int16 y, __int16 dx, __int16 dy, bool forceMotionlessPointer) {
        this->mThis = _this;
        this->mActionButtonId = actionButtonId;
        this->mButtonData = buttonData;
        this->mX = x;
        this->mY = y;
        this->mDx = dx;
        this->mDy = dy;
        this->mForceMotionlessPointer = forceMotionlessPointer;
    }
};
