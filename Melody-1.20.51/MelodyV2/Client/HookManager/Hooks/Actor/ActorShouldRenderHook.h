// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../../../SDK/Classes/Actor.h"
#include "../../../../Utils/MemoryUtils.h"

class ActorShouldRenderHook {
protected:
	using func_t = bool(__fastcall*)(Actor*);
	static inline func_t func;

	static bool Actor_ShouldRender(Actor* _this) {
		//return true;
		static AntiInvis* AntiInvisMod = (AntiInvis*)client->moduleMgr->getModule("AntiInvis");
		return AntiInvisMod->isEnabled() ? true : func(_this);
		//return func(_this);
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::ShouldRender);
		MemoryUtils::CreateHook("Actor_shouldRenderHook", address, (void*)&ActorShouldRenderHook::Actor_ShouldRender, (void*)&func);
	}
};
