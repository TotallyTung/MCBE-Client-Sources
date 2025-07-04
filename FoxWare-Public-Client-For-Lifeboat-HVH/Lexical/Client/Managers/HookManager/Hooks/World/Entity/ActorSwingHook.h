#pragma once
#include "../../FuncHook.h"

class ActorSwingHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(Actor*);
	static inline func_t oFunc;

	static void ActorSwingCallback(Actor* _this) {
		static NoSwing* noSwingMod = ModuleManager::getModule<NoSwing>();
		if (noSwingMod->isEnabled() && _this == Game::getLocalPlayer()) {
			return;
		}

		oFunc(_this);
	}
public:
	ActorSwingHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&ActorSwingCallback;
	}
};