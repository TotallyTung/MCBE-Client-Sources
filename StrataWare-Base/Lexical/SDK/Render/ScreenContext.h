#pragma once
#include "../../Utils/MemoryUtil.h"
#include "ShaderColor.h"
#include "Tessellator.h"

class ScreenContext {
public:
	CLASS_MEMBER(mce::Camera*, camera, 0x18);
	CLASS_MEMBER(ShaderColor*, currentShaderColor, 0x30);
	CLASS_MEMBER(Tessellator*, tessellator, 0xC0);
};