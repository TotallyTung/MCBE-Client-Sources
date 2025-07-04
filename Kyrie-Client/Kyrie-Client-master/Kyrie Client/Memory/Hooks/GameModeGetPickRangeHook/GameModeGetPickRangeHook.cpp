#include "GameModeGetPickRangeHook.h"
#include "../../Memory.h"
#include "../../../Client/Client.h"

typedef float(__fastcall* GetPickRange)(GameMode*, const enum InputMode*, bool);
GetPickRange _GetPickRange;

float GameModeGetPickRangeHook::getPickRange(GameMode* a1, const enum InputMode* a2, bool a3) {
	BlockReach* blockReachMod = client->moduleMgr->getModule<BlockReach>();
	if (blockReachMod->isEnabled()) return blockReachMod->blockReach;
	return _GetPickRange(a1, a2, a3);
}

void GameModeGetPickRangeHook::init(uintptr_t** VTable) {
	uintptr_t address = reinterpret_cast<uintptr_t>(VTable[10]);

	Memory::CreateHook("GameModeGetPickRangeHook", address, &GameModeGetPickRangeHook::getPickRange, &_GetPickRange);
}