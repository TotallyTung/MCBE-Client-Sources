#pragma once
#include "../FuncHook.h"

class GetFovHook : public FuncHook {
private:
	using func_t = float(__thiscall*)(LevelRendererPlayer*, float, bool);
	static inline func_t oFunc;

	static float GetFovCallback(LevelRendererPlayer* _this, float a, bool applyEffects) {

		static void* getFovForCameraReceive = (void*)MemoryUtil::findSignature("F3 0F 59 05 ?? ?? ?? ?? 48 8B C7 48 8B 5C 24");
		if (_ReturnAddress() == getFovForCameraReceive) {
			static CustomFov* customFovMod = ModuleManager::getModule<CustomFov>();
			if (customFovMod->isEnabled())
				return customFovMod->fov;
		}

		return oFunc(_this, a, applyEffects);
	}
public:
	GetFovHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&GetFovCallback;
	}
};