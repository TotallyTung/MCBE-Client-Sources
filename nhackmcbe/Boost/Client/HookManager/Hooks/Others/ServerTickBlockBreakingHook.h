#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../SDK/Classes/Player.h"

class ServerTickBlockBreakingHook {
protected:
	using func_t = void(__thiscall*)(__int64, Player*, Vec3<int>*);
	static inline func_t func;

	static void ServerPlayerBlockUseHandler_serverTickBlockBreaking(__int64 _this, Player* player, Vec3<int>* blockPos) {
		func(_this, player, blockPos);
	}
public:
	static void init() {
		uintptr_t address = findSig(Sigs::hook::ServerTickBlockBreakingHook);
		MemoryUtils::CreateHook("ServerTickBlockBreakingHook", address, (void*)&ServerTickBlockBreakingHook::ServerPlayerBlockUseHandler_serverTickBlockBreaking, (void*)&func);
	}
};