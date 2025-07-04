#include "GetFovHook.h"
#include "../../Memory.h"
#include "../../../Client/Client.h"

typedef float(__fastcall* getFov)(__int64, float, char);
getFov _getFov;

float GetFovHook::getFov(__int64 a1, float a2, char a3) {
	static CustomFov* customFovMod = client->moduleMgr->getModule<CustomFov>();
	if (customFovMod->isEnabled()) return customFovMod->fov;
	return _getFov(a1, a2, a3);
}

void GetFovHook::init() {
	uintptr_t address = Memory::getBase() + 0x13AEC70;  // 48 8B C4 48 89 58 18 48 89 70 20 57 48 81 EC ? ? ? ? 0F 29 70 E8 0F 29 78 D8 44 0F 29 40 ? 44

	Memory::CreateHook("GetFovHook", address, &GetFovHook::getFov, &_getFov);
}