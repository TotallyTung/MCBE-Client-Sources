#pragma once

#include "MinecraftTimer.h"

class Minecraft {
private:
	char pad_0x0[216]; //0x0
public:
	MinecraftTimer* mctimer; //0xD8
	MinecraftTimer* mcrenderTimer; //0xE0
};