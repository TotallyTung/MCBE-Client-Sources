#pragma once
#include "../../Utils/MemoryUtil.h"
#include "../Render/Font.h"

class MinecraftGame {
public:
	CLASS_MEMBER(bool, mouseGrabbed, 0x1A8); // CE dissect structures
	CLASS_MEMBER(Font*, mcFont, 0xF70);	// CE dissect structures
};