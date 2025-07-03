#pragma once
#include "../../FuncHook.h"

class ActorIsOnFireHook : public FuncHook {
private:
	using func_t = bool(__thiscall*)(Actor*);
	static inline func_t oFunc;

	static bool ActorIsOnFireCallback(Actor* _this) {
		static NoRender* noRenderMod = ModuleManager::getModule<NoRender>();
		if (_this == Game::getLocalPlayer() && noRenderMod->isEnabled() && noRenderMod->noFire)
			return false;

		return oFunc(_this);
	}
public:
	ActorIsOnFireHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&ActorIsOnFireCallback;
	}
};