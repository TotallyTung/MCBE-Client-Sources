#pragma once
#include "FontBitmap.h"

class MinecraftGame {
public:
	char pad_0000[400]; //0x0000
	bool canUseKeys; //0x0190
	char pad_0191[3511]; //0x0191
	FontBitmap* mcFont; //0x0F48
}; //Size: 0x0F50