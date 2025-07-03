#pragma once
#include "../../FuncHook.h"

class LevelTickHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(Level*);
	static inline func_t oFunc;
	static void LevelTickCallback(Level* _this) {
		if (Game::ourPlayer() != nullptr && Game::ourPlayer()->level == _this) {
			ModuleManager::onWorldTick(_this);
		}
		return oFunc(_this);
	}
public:
	LevelTickHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&LevelTickCallback;
	}
};