#pragma once

class DummyData {
public:
	char padding[0x120]; // Explicit padding array of 288 bytes
};

class GuiData {
public:
	char           pad_0000[0x40];        // Padding to 0x0040
    Vector2<float> mWindowResolution;     // this+0x40
    Vector2<float> mWindowResolution2;    // this+0x48
    char           pad_0050[0x8];         // Padding to 0x0050
    Vector2<float> mResolution;           // this+0x50
    char           pad_005C[0x4];         // Padding to 0x005C
    float          mScale;                // this+0x5C
    float          mScreenResRounded;     // this+0x50
    float          mSliderAmount;         // this+0x54
    float          mScalingMultiplier;    // this+0x58
    char           pad_006C[0xE];         // Padding to 0x006A
    Vector2<short> mMousePos;             // this+0x6A
public:
	void displayClientMessage(const std::string& msg)
	{
		char byte[0x28]{};
		Memory::CallFastcall<void>(SigManager::GuiData_displayClientMessage, this, msg, byte, false);
	}
};