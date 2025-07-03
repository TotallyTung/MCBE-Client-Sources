#pragma once
#include "../../../../Utils/MemoryUtils.h"

#include "../../../Client.h"

class GetCameraModeHook {
protected:
	using func_t = uint32_t(__fastcall*)(__int64);
	static inline func_t func;

	static unsigned int getCameraPerspectiveMode(__int64 a1) {
		unsigned int result = func(a1);
		mc.cameraPerspectiveMode = result;
		return result;
	}
public:
	static void init() {
		uintptr_t address = MemoryUtils::getBase() + 0x097C7A0; // This too ez to find so we dont need sig :D
		MemoryUtils::CreateHook("CauseHurtHook", address, (void*)&GetCameraModeHook::getCameraPerspectiveMode, (void*)&func);
	}
};
