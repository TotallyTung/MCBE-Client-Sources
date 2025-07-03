#pragma once
#include "../../Utils/Maths.h"

class GuiData {
private:
	char pad_0x0[0x30]; // 0x0
public:
	Vec2<float> windowSizeReal; // 0x30
	Vec2<float> windowSizeReal2; // 0x38
	Vec2<float> windowSizeScaled; // 0x40
	double guiScale1; // 0x48
	float guiScale2; // 0x50
};