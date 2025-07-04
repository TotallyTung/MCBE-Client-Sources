#include "MinecraftGameOnAppSuspendedHook.h"
#include "../../Memory.h"
#include "../../../Client/Client.h"
#include "../../../Utils/Utils.h"

typedef __int64(__fastcall* closeApp)(__int64, __int64, __int64, __int64);
closeApp _closeApp;

__int64 MinecraftGameOnAppSuspendedHook::sub_14026F320(__int64 a1, __int64 a2, __int64 a3, __int64 a4) {
	client->unject();
	Utils::DebugF("Closed App");
	return _closeApp(a1, a2, a3, a4);
}

void MinecraftGameOnAppSuspendedHook::init() {
	uintptr_t address = Memory::getBase() + 0x026F320;

	Memory::CreateHook("MinecraftGameOnAppSuspendedHook", address, &MinecraftGameOnAppSuspendedHook::sub_14026F320, &_closeApp);
}