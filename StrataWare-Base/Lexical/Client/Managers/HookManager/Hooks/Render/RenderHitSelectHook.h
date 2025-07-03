#pragma once
#include "../FuncHook.h"

class RenderHitSelectHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(LevelRendererPlayer*, BaseActorRenderContext*, BlockSource*, BlockPos*, bool);
	static inline func_t oFunc;
	static void RenderHitSelectCallback(LevelRendererPlayer* _this, BaseActorRenderContext* renderContext, BlockSource* region, BlockPos* pos, bool fancyGraphics) {
		oFunc(_this, renderContext, region, pos, fancyGraphics);
	}
public:
	RenderHitSelectHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&RenderHitSelectCallback;
	}
};