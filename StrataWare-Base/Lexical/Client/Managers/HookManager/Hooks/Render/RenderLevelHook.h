#pragma once
#include "../FuncHook.h"
#include "../../../../../Renderer/MCR.h"

class RenderLevelHook : public FuncHook {
private:
	using func_t = __int64(__thiscall*)(LevelRenderer*, ScreenContext*, __int64);
	static inline func_t oFunc;

	static __int64 LevelRenderer_renderLevelCallback(LevelRenderer* _this, ScreenContext* a2, __int64 a3) {
		__int64 result = oFunc(_this, a2, a3);
		if (Game::ourPlayer() != nullptr && _this->levelRendererPlayer != nullptr && MCR::blendMaterial != nullptr) {
			MCR::onRenderWorld(_this, a2);
		}
		return result;
	}
public:
	RenderLevelHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&LevelRenderer_renderLevelCallback;
	}
	
};