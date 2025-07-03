#pragma once
#include "../../FuncHook.h"

class PlayerSlowDownHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(__int64, __int64, __int64);
	static inline func_t oFunc;

	static void PlayerSlowDownCallback(__int64 a1, __int64 a2, __int64 a3) {
		static NoSlowDown* noSlowMod = ModuleManager::getModule<NoSlowDown>();
		if (noSlowMod->isEnabled())
			return;

		oFunc(a1, a2, a3);
	}
public:
	PlayerSlowDownHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&PlayerSlowDownCallback;
	}
	// Search float value "0.34999999" -> xrefs first Function.
};