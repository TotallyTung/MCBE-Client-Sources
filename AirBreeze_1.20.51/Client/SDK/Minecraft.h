#pragma once

#include "Classes/MinecraftUIRenderContext.h"

enum MouseAction { NONE = 0, LMB = 1, RMB = 2, WHEEL_CLICK = 3, WHEEL_SCROLL = 4 };

class Minecraft {
private:
	static ClientInstance* ci;
	static MinecraftUIRenderContext* renderCtx;
public:
	static auto getClientInstance(void)->ClientInstance*;
	static auto setClientInstance(ClientInstance*)->void;
	static auto getRenderContext(void) -> MinecraftUIRenderContext*;
	static auto setRenderContext(MinecraftUIRenderContext*) -> void;
};