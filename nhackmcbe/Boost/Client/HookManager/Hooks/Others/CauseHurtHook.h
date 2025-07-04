#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../SDK/Classes/Actor.h"

#include "../../../Client.h"

class CauseHurtHook {
protected:
	using func_t = __int64(__fastcall*)(Actor*, char, unsigned int);
	static inline func_t func;

	static __int64 CauseHurtFunc(Actor* a1, char a2, unsigned int a3) { // sub_1428200A0
		__int64 result = func(a1, a2, a3);
		static NoHurtcam* noHurtcamMod = (NoHurtcam*)client->moduleMgr->getModule("NoHurtcam");
		if (noHurtcamMod->isEnabled() && a1 == mc.getLocalPlayer() && a2 == 2) {
			uintptr_t* v5 = *(uintptr_t**)((__int64)a1 + 0x630);
			if (v5 != nullptr) {
				*v5 = 0;
			}
		}
		
		return result;
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::CauseHurtHook);
		MemoryUtils::CreateHook("CauseHurtHook", address, (void*)&CauseHurtHook::CauseHurtFunc, (void*)&func);
	}
};
