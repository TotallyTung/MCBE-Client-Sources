#pragma once
#include "../FuncHook.h"

class HurtColorHook : public FuncHook {
private:
	using func_t = mce::Color(__thiscall*)(__int64, mce::Color, __int64);
	static inline func_t oFunc;

	static mce::Color HurtColorCallback(__int64 a1, mce::Color color, __int64 a3) {
		static HurtColor* hurtColorMod = ModuleManager::getModule<HurtColor>();
		mce::Color result = oFunc(a1, color, a3);
		if (hurtColorMod->isEnabled()) {
			color.r = hurtColorMod->hurtColor.r / 255.f;
		    color.g = hurtColorMod->hurtColor.g / 255.f;
			color.b = hurtColorMod->hurtColor.b / 255.f;
			color.a = hurtColorMod->hurtColor.a / 255.f;

			return color;
		}
		return result; // The Original Color is MC_Color(1.f, 0.f, 0.f, 0.6f)
	}
public:
	HurtColorHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&HurtColorCallback;
	}
};
