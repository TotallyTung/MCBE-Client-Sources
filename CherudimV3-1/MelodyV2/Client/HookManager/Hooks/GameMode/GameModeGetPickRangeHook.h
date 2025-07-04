#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../Utils/Math.h"
#include "../../../../SDK/Classes/GameMode.h"

#include "../../../Client.h"

class GameModeGetPickRangeHook {
protected:
	using func_t = float(__fastcall*)(GameMode*, __int64, bool);
	static inline func_t oFunc;

	static float GameMode_getPickRange(GameMode* _this, __int64 inputMode, bool a3) {
		static BlockReach* blockReachMod = (BlockReach*)client->moduleMgr->getModule("BlockReach");
		if (blockReachMod->isEnabled()) return blockReachMod->blockReach;

		return oFunc(_this, inputMode, a3);
	}
public:
	static void init(uintptr_t address) {
		MemoryUtils::CreateHook("GameMode_getPickRangeHook", address, (void*)&GameModeGetPickRangeHook::GameMode_getPickRange, (void*)&oFunc);
	}
};