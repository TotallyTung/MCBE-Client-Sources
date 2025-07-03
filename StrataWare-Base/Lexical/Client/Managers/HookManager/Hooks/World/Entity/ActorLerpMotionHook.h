#pragma once
#include "../../FuncHook.h"
class ActorLerpMotionHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(Actor*, Vec3<float>*);
	static inline func_t oFunc;
	static void ActorLerpMotionCallback(Actor* _this, Vec3<float>* delta) {
		return oFunc(_this, delta);
	}
public:
	ActorLerpMotionHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&ActorLerpMotionCallback;
	}
};