#pragma once
#include "../FuncHook.h"
#include "../../../../../Renderer/MCR.h"

class RenderLevelHook : public FuncHook {
private:
	using func_t = __int64(__thiscall*)(LevelRenderer*, ScreenContext*, __int64);
	static inline func_t oFunc;

	static __int64 LevelRenderer_renderLevelCallback(LevelRenderer* _this, ScreenContext* a2, __int64 a3) {
		__int64 result = oFunc(_this, a2, a3);
		if (Game::getLocalPlayer() != nullptr && _this->levelRendererPlayer != nullptr && MCR::blendMaterial != nullptr) {
			MCR::onRenderWorld(_this, a2);
		}
		return result;
		//return oFunc(a1, a2, a3);
	}
public:
	RenderLevelHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&LevelRenderer_renderLevelCallback;
	}
	// search for "_effect" -> find function that only have move, movzx -> xref first Function -> function access a3 is LevelRenderer::renderLevel
};