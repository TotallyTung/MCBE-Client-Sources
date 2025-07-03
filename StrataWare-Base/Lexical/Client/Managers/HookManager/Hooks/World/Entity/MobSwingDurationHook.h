#pragma once
#include "../../FuncHook.h"

class SwingDurationHook : public FuncHook {
private:
	using OriginalFn = uint32_t(__thiscall*)(Mob*);
	static inline OriginalFn originalFn;
	static uint32_t Hooked_GetSwingDuration(Mob* mobInstance) {
		return originalFn(mobInstance);
	}
public:
	SwingDurationHook() {
		OriginFunc = reinterpret_cast<void**>(&originalFn);
		func = reinterpret_cast<void*>(&Hooked_GetSwingDuration);
	}
};
