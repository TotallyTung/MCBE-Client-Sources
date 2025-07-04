#pragma once
#include "../Utils/Math.h"

class GuiData {
private:
	char pad_0x0000[0x30];  //0x0000
public:
	Vec2<float> windowSizeReal;
	Vec2<float> windowSizeReal2;
	Vec2<float> windowSize;

};