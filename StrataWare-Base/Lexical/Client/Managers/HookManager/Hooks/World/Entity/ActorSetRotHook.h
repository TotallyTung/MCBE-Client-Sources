#pragma once
#include "../../FuncHook.h"

class ActorSetRotationHook : public FuncHook {
private:
	using OriginalFunction = float* (__fastcall*)(__int64, __int64, Actor*);
	static inline OriginalFunction originalFn;
	static float* Hooked_SetRotation(__int64 param1, __int64 param2, Actor* actor) {
		LocalPlayer* localPlayer = Game::ourPlayer();
		if (actor == localPlayer) {
			float* result = originalFn(param1, param2, actor);
			ModuleManager::onUpdateRotation(localPlayer);
			return result;
		}
		return originalFn(param1, param2, actor);
	}
public:
	ActorSetRotationHook() {
		OriginFunc = reinterpret_cast<void**>(&originalFn);
		func = reinterpret_cast<void*>(&Hooked_SetRotation);
	}
};
