#pragma once
#include "../FuncHook.h"

class HurtColorHook : public FuncHook {
private:
	using func_t = mce::Color(__thiscall*)(__int64, mce::Color, __int64);
	static inline func_t oFunc;
	static mce::Color HurtColorCallback(__int64 a1, mce::Color color, __int64 a3) {
		mce::Color result = oFunc(a1, color, a3);
		return result;
	}
public:
	HurtColorHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&HurtColorCallback;
	}
};
