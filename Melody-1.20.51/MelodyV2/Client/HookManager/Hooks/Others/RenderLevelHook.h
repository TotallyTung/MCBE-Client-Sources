// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../../../Utils/MemoryUtils.h"

#include "../../../Client.h"

class RenderLevelHook {
protected:
	using func_t = __int64(__fastcall*)(__int64, ScreenContext*, __int64);
	static inline func_t func;

	static __int64 LevelRenderer_renderLevel(__int64 a1, ScreenContext* a2, __int64 a3) {
		__int64 result = func(a1, a2, a3);
		if (RenderUtils::blendMaterial != nullptr) {
			RenderUtils::setGameRenderContext(a2);
			client->moduleMgr->onLevelRender();
			RenderUtils::setGameRenderContext(nullptr);
		}
		return result;
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::RenderLevelHook);
		MemoryUtils::CreateHook("RenderLevelHook", address, (void*)&RenderLevelHook::LevelRenderer_renderLevel, (void*)&func);
	}
};
