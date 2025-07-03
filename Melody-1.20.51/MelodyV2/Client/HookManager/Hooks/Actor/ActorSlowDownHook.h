// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../../../SDK/Classes/Actor.h"
#include "../../../../SDK/Classes/StrictEntityContext.h"
#include "../../../../Utils/MemoryUtils.h"
#include "../../../ModuleManager/Modules/Movement/NoSlowDown.h"

class ActorSlowDownHook {
protected:
	using func_t = void(__fastcall*)(void*, StrictEntityContext*, void*, void*);
	static inline func_t func;

	static void Actor_SlowDown(void* dont_care, StrictEntityContext* context, void* lol, void* lol2) {
		//return true;
		static NoSlowDown* NoSlowDownMod = (NoSlowDown*)client->moduleMgr->getModule("NoSlowDown");
		if (NoSlowDownMod->isEnabled())
		{
			return;
		}
		return func(dont_care, context, lol, lol2);
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::NoSlowDownHook);
		MemoryUtils::CreateHook("Actor_SlowDownHook", address, (void*)&ActorSlowDownHook::Actor_SlowDown, (void*)&func);
	}
};
