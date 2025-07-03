#pragma once
#include <Hook/Hook.hpp>
//
// Created by vastrakai on 6/29/2024.
//

class MouseHook : public Hook
{
    // MouseDevice *this, char actionButtonId, char buttonData, __int16 x, __int16 y, __int16 dx, __int16 dy, bool forceMotionlessPointer)
public:
    MouseHook() : Hook() {
        mName = "MouseDevice::feed";
    }

    static std::unique_ptr<Detour> mDetour;

    static inline std::map<char, bool> mButtonStates;

    static void onMouse(void* _this, char actionButtonId, int buttonData, __int16 x, __int16 y, __int16 dx, __int16 dy, uint8_t forceMotionlessPointer);
    static void simulateMouseInput(char actionButtonId, int buttonData, short x = 0, short y = 0, short dx = 0, short dy = 0);
    void init() override;
};

