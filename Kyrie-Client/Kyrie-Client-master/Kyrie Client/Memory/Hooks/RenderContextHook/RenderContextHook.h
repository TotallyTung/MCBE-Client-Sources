#pragma once
#include "../../../SDK/MinecraftUIRenderContext.h"

class RenderContextHook {
private:
	static __int64 RenderContextCallBack(__int64 a1, MinecraftUIRenderContext* renderCtx);
public:
	static void init();
};