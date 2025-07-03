#pragma once
#include "../../FuncHook.h"

class SlowDownHook : public FuncHook {
private:
	using OriginalFn = void(__thiscall*)(__int64, __int64, __int64);
	static inline OriginalFn originalFn;
	static void Hooked_SlowDown(__int64 param1, __int64 param2, __int64 param3) {
		originalFn(param1, param2, param3);
	}
public:
	SlowDownHook() {
		OriginFunc = reinterpret_cast<void**>(&originalFn);
		func = reinterpret_cast<void*>(&Hooked_SlowDown);
	}
};
