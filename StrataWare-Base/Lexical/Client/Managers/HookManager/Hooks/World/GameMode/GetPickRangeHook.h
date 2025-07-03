#pragma once
#include "../../FuncHook.h"

class PickRangeHook : public FuncHook {
private:
	using OriginalFn = float(__thiscall*)(GameMode*, const InputMode&, bool);
	static inline OriginalFn originalFn;
	static float Hooked_GetPickRange(GameMode* gameMode, const InputMode& inputMode, bool isVR) {
		return originalFn(gameMode, inputMode, isVR);
	}

public:
	PickRangeHook() {
		OriginFunc = reinterpret_cast<void**>(&originalFn);
		func = reinterpret_cast<void*>(&Hooked_GetPickRange);
	}
};
