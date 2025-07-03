#pragma once
#include "../../FuncHook.h"

class ActorNormalTickHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(Actor*);
	static inline func_t oFunc;

	static void ActorNormalTickCallback(Actor* _this) {
		LocalPlayer* localPlayer = Game::ourPlayer();
		if (localPlayer == _this) {
			ModuleManager::onNormalTick(localPlayer);
		}

		oFunc(_this);
	}
public:
	ActorNormalTickHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&ActorNormalTickCallback;
	}
};