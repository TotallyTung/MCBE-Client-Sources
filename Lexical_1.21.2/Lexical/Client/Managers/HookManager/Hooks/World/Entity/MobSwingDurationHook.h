#pragma once
#include "../../FuncHook.h"

class MobSwingDurationHook : public FuncHook {
private:
	using func_t = uint32_t(__thiscall*)(Mob*);
	static inline func_t oFunc;

	static uint32_t MobGetCurrentSwingDurationCallback(Mob* _this) {
		static Swing* swingMod = ModuleManager::getModule<Swing>();
		if (swingMod->isEnabled() && Game::getLocalPlayer() == _this) {
			return swingMod->swingSpeed;
		}

		return oFunc(_this);
	}
public:
	MobSwingDurationHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&MobGetCurrentSwingDurationCallback;
	}
};