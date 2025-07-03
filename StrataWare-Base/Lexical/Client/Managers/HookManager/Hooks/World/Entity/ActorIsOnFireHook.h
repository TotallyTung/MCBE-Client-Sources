#pragma once
#include "../../FuncHook.h"

class ActorIsOnFireHook : public FuncHook {
private:
	using func_t = bool(__thiscall*)(Actor*);
	static inline func_t oFunc;
	static bool ActorIsOnFireCallback(Actor* _this) {
		return oFunc(_this);
	}
public:
	ActorIsOnFireHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&ActorIsOnFireCallback;
	}
};