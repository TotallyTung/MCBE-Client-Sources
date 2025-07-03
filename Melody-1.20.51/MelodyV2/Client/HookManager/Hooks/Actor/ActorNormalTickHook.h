// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../../../SDK/Classes/Actor.h"
#include "../../../../Utils/MemoryUtils.h"

class ActorNormalTickHook {
protected:
	using func_t = void(__fastcall*)(Actor*);
	static inline func_t func;
	
	static void callback(Actor* _this) {
		if (_this == mc.getLocalPlayer()) client->moduleMgr->onNormalTick(_this);
		func(_this);
	}
public:
	static void init(uintptr_t address) {
		MemoryUtils::CreateHook("Actor_NormalTickHook", address, (void*)&ActorNormalTickHook::callback, (void*)&func);
	}
};
