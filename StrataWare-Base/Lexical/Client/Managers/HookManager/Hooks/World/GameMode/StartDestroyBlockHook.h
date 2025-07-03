#pragma once
#include "../../FuncHook.h"
class StartDestroyBlockHook : public FuncHook {
private:
	using func_t = bool(__thiscall*)(GameMode*, BlockPos*, uint8_t, bool*);
	static inline func_t oFunc;

	static bool StartDestroyBlockCallback(GameMode* _this, BlockPos* pos, uint8_t face, bool* hasDestroyedBlock) {
		return oFunc(_this, pos, face, hasDestroyedBlock);
	}
public:
	StartDestroyBlockHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&StartDestroyBlockCallback;
	}
};