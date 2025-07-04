#pragma once
#include "../../FuncHook.h"

class LevelTickHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(Level*);
	static inline func_t oFunc;

	static void LevelTickCallback(Level* _this) {
		if (Game::getLocalPlayer() != nullptr && Game::getLocalPlayer()->level == _this) {
			ModuleManager::onLevelTick(_this);
		}
		return oFunc(_this);
	}
public:
	LevelTickHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&LevelTickCallback;
	}
};