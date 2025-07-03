// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../../../SDK/Classes/Actor.h"
#include "../../../../Utils/MemoryUtils.h"

#include "../../../Client.h"

class ActorIsOnFireHook {
protected:
	using func_t = bool(__fastcall*)(Actor*);
	static inline func_t func;

	static bool Actor_IsOnFire(Actor* _this) {
		static NoRender* noRenderMod = (NoRender*)client->moduleMgr->getModule("NoRender");
		if (noRenderMod->isEnabled() && noRenderMod->noFire && _this == (Actor*)mc.getLocalPlayer()) return false;
		return func(_this);
	}
public:
	static void init(uintptr_t address) {
		MemoryUtils::CreateHook("Actor_isOnFireHook", address, (void*)&ActorIsOnFireHook::Actor_IsOnFire, (void*)&func);
	}
};
