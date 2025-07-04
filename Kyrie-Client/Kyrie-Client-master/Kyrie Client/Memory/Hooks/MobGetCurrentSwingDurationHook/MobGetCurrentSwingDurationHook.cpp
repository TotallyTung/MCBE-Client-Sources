#include "MobGetCurrentSwingDurationHook.h"
#include "../../../Client/Client.h"
#include "../../Memory.h"

typedef int(__fastcall* getSwingDuration)(Mob*);
getSwingDuration _getSwingDuration;

int MobGetCurrentSwingDurationHook::MobGetCurrentSwingDuration(Mob* a1) {
	static Swing* swingMod = client->moduleMgr->getModule<Swing>();
	if (swingMod->isEnabled() && a1 == (Mob*)mc.getLocalPlayer()) return swingMod->speed;
	return _getSwingDuration(a1);
}

void MobGetCurrentSwingDurationHook::init() {
	uintptr_t address = Memory::getBase() + 0x30652F0; // 48 89 5C 24 ? 57 48 83 EC 20 48 8B 15 ? ? ? ? 48 8B F9 33 DB

	Memory::CreateHook("MobGetCurrentSwingDurationHook", address, &MobGetCurrentSwingDurationHook::MobGetCurrentSwingDuration, &_getSwingDuration);
}