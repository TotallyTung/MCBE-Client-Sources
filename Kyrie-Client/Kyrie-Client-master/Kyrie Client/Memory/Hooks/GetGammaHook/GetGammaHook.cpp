#include "GetGammaHook.h"
#include "../../Memory.h"
#include "../../../Client/Client.h"

typedef float(__fastcall* getGamma)(uintptr_t*);
getGamma _getGamma;

float GetGammaHook::getGamma(uintptr_t* a1) {
	static FullBright* fullBrightMod = client->moduleMgr->getModule<FullBright>();
	if (fullBrightMod->isEnabled()) return 12.f;
	return _getGamma(a1);
}

void GetGammaHook::init() {
	uintptr_t address = Memory::getBase() + 0x9045D0;  // 48 83 EC 28 80 B9 ? ? ? ? ? 48 8D 54 24 ? 48 8B 01 48 8B 40 60

	Memory::CreateHook("GetGammaHook", address, &GetGammaHook::getGamma, &_getGamma);
}