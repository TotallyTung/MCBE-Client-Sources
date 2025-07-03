#pragma once
#include "../../FuncHook.h"

class ActorSetRotHook : public FuncHook {
private:
	using func_t = float*(__fastcall*)(__int64, __int64, Actor*);
	static inline func_t oFunc;

	static float* ActorSetRotCallback(__int64 a1, __int64 a2, Actor* a3) {
		LocalPlayer* localPlayer = Game::getLocalPlayer();
		if (localPlayer == a3) {
			float* result = oFunc(a1, a2, a3);
			ModuleManager::onUpdateRotation(localPlayer);
			return result;
		}
		return oFunc(a1, a2, a3);
	}
public:
	ActorSetRotHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&ActorSetRotCallback;
	}
};