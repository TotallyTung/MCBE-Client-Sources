#pragma once
#include "../../../../SDK/Classes/Options.h"
#include "../../../../Utils/MemoryUtils.h"

#include "../../../Client.h"

class GetFogColorHook {
protected:
public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::GetFogColorHook);
	}
};
