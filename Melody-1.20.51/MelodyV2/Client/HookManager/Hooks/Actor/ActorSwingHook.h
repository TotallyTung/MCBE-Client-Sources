// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../../../SDK/Classes/Actor.h"
#include "../../../../Utils/MemoryUtils.h"

#include "../../../Client.h"

class ActorSwingHook {
protected:
	using func_t = void(__fastcall*)(Actor*);
	static inline func_t func;

	static void Actor_Swing(Actor* _this) {
		func(_this);
	}
public:
	static void init(uintptr_t address) {
		MemoryUtils::CreateHook("Actor_swingHook", address, (void*)&ActorSwingHook::Actor_Swing, (void*)&func);
	}
};
