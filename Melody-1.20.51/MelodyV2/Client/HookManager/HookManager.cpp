// Created by Tony on 2024-10-10 10:01:54

#include "HooksManager.h"

#include "Hooks/DirectX/DirectXHook.h"
#include "../../Libs/ImFX/imfx.h"

#include "Hooks/ScreenView/SetUpAndRenderHook.h"

#include "Hooks/Key/KeyMapHook.h"
#include "Hooks/Key/KeyMouseHook.h"

#include "Hooks/Others/CauseHurtHook.h"
#include "Hooks/Others/FontDrawTransformedHook.h"
#include "Hooks/Others/GetCameraModeHook.h"
#include "Hooks/Others/GetFovHook.h"
#include "Hooks/Others/GetFogColorHook.h"
#include "Hooks/Others/GetGammaHook.h"
#include "Hooks/Others/HIDControllerHook.h"
#include "Hooks/Others/HurtColorHook.h"
#include "Hooks/Others/GlintColorHook.h"
#include "Hooks/Others/RenderLevelHook.h"
#include "Hooks/Others/ServerTickBlockBreakingHook.h"
#include "Hooks/Others/onContainerScreenControllerTickEventHook.h"

#include "Hooks/Mob/GetCurrentSwingDurationHook.h"

#include "Hooks/Actor/ActorGetBodyYawHook.h"
#include "Hooks/Actor/ActorServerRots.h"
#include "Hooks/Actor/ActorGetHeadRotHook.h"
#include "Hooks/Actor/ActorNormalTickHook.h"
#include "Hooks/Actor/ActorIsOnFireHook.h"
#include "Hooks/Actor/ActorShouldRenderHook.h"
#include "Hooks/Actor/ActorLerpMotionHook.h"
#include "Hooks/Actor/ActorSwingHook.h"
#include "Hooks/Actor/ActorSlowDownHook.h"

#include "Hooks/GameMode/GameModeStartDestroyBlockHook.h"
#include "Hooks/GameMode/GameModeStopDestroyBlockHook.h"
#include "Hooks/GameMode/GameModeGetPickRangeHook.h"

void HookManager::init() {
	MH_Initialize();
	DirectXHook::init();
	SetUpAndRenderHook::init();
	KeyMapHook::init();
	KeyMouseHook::init();
	GetGlintColorHook::init();
	CauseHurtHook::init();
	//ActorServerRots::init();
	FontDrawTransformedHook::init();
	GetCameraModeHook::init();
	GetFovHook::init();
	GetFogColorHook::init();
	GetGammaHook::init();
	HIDControllerHook::init();
	HurtColorHook::init();
	RenderLevelHook::init();
	ActorShouldRenderHook::init();
	onContainerScreenControllerTickEventHook::init();
	//ServerTickBlockBreakingHook::init(); // Dinh dung func nay lam Packetmine ma no ncc
	GetCurrentSwingDurationHook::init();
	ActorSlowDownHook::init();

	// Actor Vtables
	{ 
		uintptr_t** PlayerVTable = MemoryUtils::getVtableFromSignature(Sigs::vtable::PlayerVtable, 3);
		ActorGetHeadRotHook::init((uintptr_t)PlayerVTable[17]);
		ActorGetBodyYawHook::init((uintptr_t)PlayerVTable[18]);
		ActorLerpMotionHook::init((uintptr_t)PlayerVTable[27]);
		ActorNormalTickHook::init((uintptr_t)PlayerVTable[29]);
		ActorIsOnFireHook::init((uintptr_t)PlayerVTable[58]);
		//ActorShouldRenderHook::init((uintptr_t)PlayerVTable[75]);
		ActorSwingHook::init((uintptr_t)PlayerVTable[132]);
	}
	// GameMode Vtables
	{
		uintptr_t** GameModeVTable = MemoryUtils::getVtableFromSignature(Sigs::vtable::GamemodeVtable, 3);
		GameModeStartDestroyBlock::init((uintptr_t)GameModeVTable[1]);
		GameModeStopDestroyBlock::init((uintptr_t)GameModeVTable[4]);
		GameModeGetPickRangeHook::init((uintptr_t)GameModeVTable[10]);
	}
}

void HookManager::Restore() {
	kiero::shutdown();
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
	ImFX::CleanupFX();
}