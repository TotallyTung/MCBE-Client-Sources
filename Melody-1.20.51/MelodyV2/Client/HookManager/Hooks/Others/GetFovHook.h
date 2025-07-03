// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../../../Utils/MemoryUtils.h"

#include "../../../Client.h"

class GetFovHook {
protected:
	using func_t = float(__thiscall*)(LevelRendererPlayer*, float, bool);
	static inline func_t func;

	static float LevelRendererPlayer_getFov(LevelRendererPlayer* _this, float a, bool applyEffects) {
		static CustomFov* customFovMod = (CustomFov*)client->moduleMgr->getModule("CustomFov");
		if (customFovMod->isEnabled()) {
			return customFovMod->fov;
		}
		return func(_this, a, applyEffects);
	}
public:
	static void init() {
		MemoryUtils::CreateHook("CauseHurtHook", findSig(Sigs::hook::getFovHook), (void*)&GetFovHook::LevelRendererPlayer_getFov, (void*)&func);
	}
};
