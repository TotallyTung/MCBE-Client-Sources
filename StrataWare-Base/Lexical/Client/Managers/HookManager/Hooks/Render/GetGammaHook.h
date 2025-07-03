#pragma once
#include "../FuncHook.h"

class GetGammaHook : public FuncHook {
private:
	using func_t = float(__thiscall*)(__int64);
	static inline func_t oFunc;

	static void initializeNoRenderSettings(__int64 a1) {
		uintptr_t** list = (uintptr_t**)a1;
	}

	static float GetGammaCallback(__int64 a1) {
		static bool initPtr = false;
		if (!initPtr) {
			initializeNoRenderSettings(a1);
			initPtr = true;
		}
		return oFunc(a1);
	}

public:
	GetGammaHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&GetGammaCallback;
	}
};
