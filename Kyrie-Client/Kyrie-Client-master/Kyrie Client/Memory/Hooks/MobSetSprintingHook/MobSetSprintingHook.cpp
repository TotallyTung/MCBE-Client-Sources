#include "MobSetSprintingHook.h"
#include "../../Memory.h"

typedef __int64(__fastcall* SetSprinting)(Mob*, unsigned __int8);
SetSprinting _SetSprinting;

__int64 MobSetSprintingHook::setSprinting(Mob* a1, unsigned __int8 a2) {
	if (a2 == false) return 0;
	return _SetSprinting(a1, a2);
}

void MobSetSprintingHook::init() {
	uintptr_t address = Memory::getBase() + 0x3064550;

	Memory::CreateHook("MobSetSprintingHook", address, &MobSetSprintingHook::setSprinting, &_SetSprinting);
}