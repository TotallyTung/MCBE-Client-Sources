#pragma once
#include "../../../../SDK/Classes/Actor.h"
#include "../../../../Utils/MemoryUtils.h"

#include "../../../Client.h"

class ActorGetHeadRotHook {
protected:
	using func_t = float(__thiscall*)(Actor*, float);
	static inline func_t func;

	static float Actor_getInterpolatedHeadRot(Actor* _this, float a1) { // Head Yaw
		if (_this == mc.getLocalPlayer()) {
			// Soon
		}
		return func(_this, a1);
	}
public:
	static void init(uintptr_t address) {
		MemoryUtils::CreateHook("Actor_getInterpolatedHeadRotHook", address, (void*)&ActorGetHeadRotHook::Actor_getInterpolatedHeadRot, (void*)&func);
	}
};