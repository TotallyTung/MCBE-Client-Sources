#pragma once
#include "../../FuncHook.h"

class ActorLerpMotionHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(Actor*, Vec3<float>*);
	static inline func_t oFunc;

	static void ActorLerpMotionCallback(Actor* _this, Vec3<float>* delta) {

		static Velocity* velocityMod = ModuleManager::getModule<Velocity>();
		if (velocityMod->isEnabled() && Game::getLocalPlayer() == _this) {
			static void* networkSender = reinterpret_cast<void*>(MemoryUtil::findSignature("48 8D 57 38 48 8B CE 48") + 0x14);
			if (_ReturnAddress() == networkSender) {
				float hPercent = velocityMod->horizontal / 100.f;
				float vPercent = velocityMod->vertical / 100.f;
				*delta = _this->stateVector->velocity.lerp(*delta, hPercent, vPercent, hPercent);
			}
		}

		return oFunc(_this, delta);
	}
public:
	ActorLerpMotionHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&ActorLerpMotionCallback;
	}
};