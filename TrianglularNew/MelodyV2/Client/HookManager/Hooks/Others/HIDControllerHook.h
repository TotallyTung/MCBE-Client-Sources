#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../SDK/Classes/HIDController.h"

#include "../../../Client.h"

class HIDControllerHook {
protected:
	using func_t = __int64(__fastcall*)(HIDController*, __int64, __int64);
	static inline func_t func;

	static __int64 HIDController_keyMouse(HIDController* _this, __int64 a2, __int64 a3) {
		mc.setHIDController(_this);
		return func(_this, a2, a3);
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::HIDControllerHook);
		MemoryUtils::CreateHook("HIDControllerHook", address, (void*)&HIDControllerHook::HIDController_keyMouse, (void*)&func);
	}
};
