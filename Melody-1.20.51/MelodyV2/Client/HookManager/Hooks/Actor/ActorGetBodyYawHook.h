// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../../../SDK/Classes/Actor.h"
#include "../../../../Utils/MemoryUtils.h"

#include "../../../Client.h"

class ActorGetBodyYawHook {
protected:
	using func_t = float(__thiscall*)(Actor*, float);
	static inline func_t func;

	static float Actor_getInterpolatedBodyYaw(Actor* _this, float a1) { // Body Yaw
		if (_this == mc.getLocalPlayer()) {
			// Soon
		}
		return func(_this, a1);
	}
public:
	static void init(uintptr_t address) {
		MemoryUtils::CreateHook("Actor_getInterpolatedBodyYawHook", address, (void*)&ActorGetBodyYawHook::Actor_getInterpolatedBodyYaw, (void*)&func);
	}
};