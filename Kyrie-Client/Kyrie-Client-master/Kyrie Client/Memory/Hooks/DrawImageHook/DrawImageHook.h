#pragma once
#include "../../../SDK/TexturePtr.h"
#include "../../../SDK/MinecraftUIRenderContext.h"

class DrawImageHook {
private:
	static __int64 DrawImageCallBack(MinecraftUIRenderContext* renderCtx, TexturePtr* texturePtr, float* a3, float* a4, __int64 a5, __int64 a6);
public:
	static void init();
};