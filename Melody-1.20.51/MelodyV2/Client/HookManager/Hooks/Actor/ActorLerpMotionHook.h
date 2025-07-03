// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../../../SDK/Classes/Actor.h"
#include "../../../../Utils/MemoryUtils.h"

#include "../../../Client.h"
#include <intrin.h>

class ActorLerpMotionHook {
protected:
	using func_t = bool(__fastcall*)(Actor*, Vec3<float>);
	static inline func_t func;

	static void Actor_LerpMotion(Actor* _this, Vec3<float> a2) {

		static Velocity* velocityMod = (Velocity*)client->moduleMgr->getModule("Velocity");
		if (velocityMod->isEnabled() && _this == mc.getLocalPlayer()) {
			static void* networkSender = reinterpret_cast<void*>(16 + findSig(Sigs::hook::ActorLerpMotionHook));
			if (networkSender == _ReturnAddress()) a2 = _this->stateVectorComponent->velocity.lerp(a2, velocityMod->xzModifier, velocityMod->yModifier, velocityMod->xzModifier);
		}

		func(_this, a2);
	}

public:
	static void init(uintptr_t address) {
		MemoryUtils::CreateHook("Actor_LerpMotion", address, (void*)&ActorLerpMotionHook::Actor_LerpMotion, (void*)&func);
	}
};