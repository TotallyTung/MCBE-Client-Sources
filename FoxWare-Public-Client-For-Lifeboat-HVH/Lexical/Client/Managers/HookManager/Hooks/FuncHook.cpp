#include "FuncHook.h"
#include <MinHook.h>

bool FuncHook::enableHook() {
	if (!enable) {
		if (!address) {
			logF("[%s] Invalid address!", name);
			return false;
		}
		if (MH_CreateHook((void*)address, func, reinterpret_cast<LPVOID*>(OriginFunc)) != MH_OK) {
			logF("[%s] Failed to create hook!", name);
			return false;
		}
		if (MH_EnableHook((void*)address) != MH_OK) {
			logF("[%s] Failed to enable hook!", name);
			return false;
		}
		//logF("[%s] Successfully hooked!", name);
		enable = true;
		return true;
	}
	return true;
}

void FuncHook::onHookRequest() {
}