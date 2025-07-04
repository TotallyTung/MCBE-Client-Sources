#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../SDK/Classes/Mob.h"

#include "../../../Client.h"

class GetCurrentSwingDurationHook {
protected:
	using func_t = int(__thiscall*)(Mob* _this);
	static inline func_t func;

	static int Mob_GetCurrentSwingDuration(Mob* _this) {
		static Swing* swingMod = (Swing*)client->moduleMgr->getModule("Swing");
		if (swingMod->isEnabled() && _this == (Mob*)mc.getLocalPlayer()) {
			return swingMod->swingSpeed;
		}
		return func(_this);
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::GetCurrentSwingDurationHook);
		MemoryUtils::CreateHook("Mob_GetCurrentSwingDurationHook", address, (void*)&GetCurrentSwingDurationHook::Mob_GetCurrentSwingDuration, (void*)&func);
	}
};
