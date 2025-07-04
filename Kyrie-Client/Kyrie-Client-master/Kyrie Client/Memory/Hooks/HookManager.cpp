#include "HookManager.h"
#include "../Memory.h"
#include <MinHook.h>

#include "RenderContextHook/RenderContextHook.h"
#include "KeyMapHook/KeyMapHook.h"
#include "KeyMouseHook/KeyMouseHook.h"
#include "DrawImageHook/DrawImageHook.h"
#include "GetGammaHook/GetGammaHook.h"
//#include "MobSetSprintingHook/MobSetSprintingHook.h"
#include "MobGetCurrentSwingDurationHook/MobGetCurrentSwingDurationHook.h"
#include "GameModeGetPickRangeHook/GameModeGetPickRangeHook.h"
//#include "MinecraftGameOnAppSuspendedHook/MinecraftGameOnAppSuspendedHook.h"
#include "GetFovHook/GetFovHook.h"
#include "LevelRendererPlayerUpdateCameraHook/LevelRendererPlayerUpdateCameraHook.h"

#include "SwapChainHook/SwapChainHook.h"

void HookManager::initAll() {
	//MinecraftGameOnAppSuspendedHook::init();
	RenderContextHook::init(); // always init this the renderContextHook first to have clientInstance!
	KeyMapHook::init();
	KeyMouseHook::init();
	DrawImageHook::init();
	GetGammaHook::init();
	//MobSetSprintingHook::init();
	GetFovHook::init();
	MobGetCurrentSwingDurationHook::init();
	LevelRendererPlayerUpdateCameraHook::init();

	uintptr_t gameModeOffset = Memory::getBase() + 0x2FCFD3F; // 48 8D 05 ? ? ? ? 48 89 01 48 89 51 ? 48 C7 41 ? ? ? ? ? C7 41 18 ? ? ? ? 44 88 61 ? 4C 89 61
	if (gameModeOffset != 0x0) {
		int offset = *(int*)(gameModeOffset + 3);
		uintptr_t** gameModeVtable = (uintptr_t**)(gameModeOffset + offset + 7);
		if (gameModeVtable != 0x0) {
			GameModeGetPickRangeHook::init(gameModeVtable);
		}
	}

	SwapChainHook::init(); // Init this LAST!
}

void HookManager::RestoreAll() {
	SwapChainHook::unject();
	MH_DisableHook(MH_ALL_HOOKS);
	Sleep(10);
	MH_RemoveHook(MH_ALL_HOOKS);
	Sleep(10);
	MH_Uninitialize();
}